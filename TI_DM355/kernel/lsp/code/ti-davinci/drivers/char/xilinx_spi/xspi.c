/******************************************************************************
*
*     Author: Xilinx, Inc.
*
*
*     This program is free software; you can redistribute it and/or modify it
*     under the terms of the GNU General Public License as published by the
*     Free Software Foundation; either version 2 of the License, or (at your
*     option) any later version.
*
*
*     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
*     COURTESY TO YOU. BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
*     ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION OR STANDARD,
*     XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION IS FREE
*     FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE FOR OBTAINING
*     ANY THIRD PARTY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
*     XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
*     THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY
*     WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM
*     CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND
*     FITNESS FOR A PARTICULAR PURPOSE.
*
*
*     Xilinx hardware products are not intended for use in life support
*     appliances, devices, or systems. Use in such applications is
*     expressly prohibited.
*
*
*     (c) Copyright 2002 Xilinx Inc.
*     All rights reserved.
*
*
*     You should have received a copy of the GNU General Public License along
*     with this program; if not, write to the Free Software Foundation, Inc.,
*     675 Mass Ave, Cambridge, MA 02139, USA.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xspi.c
*
* Contains required functions of the XSpi driver component.  See xspi.h for
* a detailed description of the device and driver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rpm  10/11/01 First release
* 1.00b jhl  03/14/02 Repartitioned driver for smaller files.
* 1.00b rpm  04/25/02 Collapsed IPIF and reg base addresses into one
* 1.00b rmm  05/14/03 Fixed diab compiler warnings relating to asserts
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xspi.h"
#include "xspi_i.h"
#include "xipif_v1_23_b.h"
#include "xio.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

static void StubStatusHandler(void *CallBackRef, XStatus StatusEvent,
			      unsigned int ByteCount);

/************************** Variable Definitions *****************************/

/*****************************************************************************/
/**
*
* Initializes a specific XSpi instance such that the driver is ready to use.
*
* The state of the device after initialization is:
*   - Device is disabled
*   - Slave mode
*   - Active high clock polarity
*   - Clock phase 0
*
* @param    InstancePtr is a pointer to the XSpi instance to be worked on.
* @param    DeviceId is the unique id of the device controlled by this XSpi
*           instance. Passing in a device id associates the generic XSpi
*           instance to a specific device, as chosen by the caller or
*           application developer.
*
* @return
*
* The return value is XST_SUCCESS if successful.  On error, a code indicating
* the specific error is returned.  Possible error codes are:
* - XST_DEVICE_IS_STARTED if the device is started. It must be stopped to
*   re-initialize.
* - XST_DEVICE_NOT_FOUND if the device was not found in the configuration such
*   that initialization could not be accomplished.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XSpi_Initialize(XSpi * InstancePtr, u16 DeviceId)
{
	XSpi_Config *SpiConfigPtr;	/* Pointer to Configuration ROM data */

	XASSERT_NONVOID(InstancePtr != NULL);

	/*
	 * If the device is started, disallow the initialize and return a status
	 * indicating it is started.  This allows the user to stop the device
	 * and reinitialize, but prevents a user from inadvertently initializing
	 */
	if (InstancePtr->IsStarted == XCOMPONENT_IS_STARTED) {
		return XST_DEVICE_IS_STARTED;
	}

	/*
	 * Lookup the device configuration in the temporary CROM table. Use this
	 * configuration info down below when initializing this component.
	 */
	SpiConfigPtr = XSpi_LookupConfig(DeviceId);
	if (SpiConfigPtr == NULL) {
		return XST_DEVICE_NOT_FOUND;
	}

	/*
	 * Set some default values
	 */
	InstancePtr->IsStarted = 0;
	InstancePtr->IsBusy = FALSE;

	InstancePtr->BaseAddr = SpiConfigPtr->BaseAddress;
	InstancePtr->StatusHandler = StubStatusHandler;

	InstancePtr->SendBufferPtr = NULL;
	InstancePtr->RecvBufferPtr = NULL;
	InstancePtr->RequestedBytes = 0;
	InstancePtr->RemainingBytes = 0;
	InstancePtr->HasFifos = SpiConfigPtr->HasFifos;
	InstancePtr->SlaveOnly = SpiConfigPtr->SlaveOnly;
	InstancePtr->NumSlaveBits = SpiConfigPtr->NumSlaveBits;
	InstancePtr->IsReady = XCOMPONENT_IS_READY;

	/* Create a slave select mask based on the number of bits that can
	 * be used to deselect all slaves, initialize the value to put into
	 * the slave select register to this value
	 */
	InstancePtr->SlaveSelectMask = (1 << InstancePtr->NumSlaveBits) - 1;
	InstancePtr->SlaveSelectReg = InstancePtr->SlaveSelectMask;

	/*
	 * Clear the statistics for this driver
	 */
	XSpi_mClearStats(InstancePtr);

	/*
	 * Reset the SPI device to get it into its initial state. It is expected
	 * that device configuration will take place after this initialization is
	 * done, but before the device is started. Be sure to call the version's
	 * Reset function since it does not require the IsReady flag to be set yet.
	 */
	XSpi_Reset(InstancePtr);

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function enables interrupts for the SPI device. It is up to the user to
* connect the SPI interrupt handler to the interrupt controller before this
* Start function is called. The GetIntrHandler function is used for that
* purpose. If the device is configured with FIFOs, the FIFOs are reset at
* this time.
*
* @param    InstancePtr is a pointer to the XSpi instance to be worked on.
*
* @return
*
* XST_SUCCESS if the device is successfully started, or XST_DEVICE_IS_STARTED
* if the device was already started.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XSpi_Start(XSpi * InstancePtr)
{
	u16 ControlReg;

	XASSERT_NONVOID(InstancePtr != NULL);
	XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	/*
	 * If it is already started, return a status indicating so
	 */
	if (InstancePtr->IsStarted == XCOMPONENT_IS_STARTED) {
		return XST_DEVICE_IS_STARTED;
	}

	/*
	 * Enable interrupts in the IPIF (connecting to the interrupt controller
	 * and enabling interrupts there is the responsibility of the caller)
	 */
	XIIF_V123B_WRITE_IIER(InstancePtr->BaseAddr, XSP_INTR_DFT_MASK);

	/*
	 * Indicate that the device is started before we enable the transmitter
	 * or receiver or interrupts.
	 */
	InstancePtr->IsStarted = XCOMPONENT_IS_STARTED;

	/*
	 * Reset the transmit and receive FIFOs if present. There is a critical
	 * section here since this register is also modified during interrupt
	 * context.  So we wait until after the r/m/w of the control register to
	 * enable the global IPIF interrupt.
	 */
	ControlReg = XIo_In16(InstancePtr->BaseAddr + XSP_CR_OFFSET);
	ControlReg |= XSP_CR_TXFIFO_RESET_MASK | XSP_CR_RXFIFO_RESET_MASK |
	    XSP_CR_ENABLE_MASK;
	XIo_Out16(InstancePtr->BaseAddr + XSP_CR_OFFSET, ControlReg);

	/*
	 * Enable the global IPIF interrupt just after we start.
	 */
	XIIF_V123B_GINTR_ENABLE(InstancePtr->BaseAddr);

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function stops the SPI device by disabling interrupts and disabling the
* device itself. Interrupts are disabled only within the device itself. If
* desired, the caller is responsible for disabling interrupts in the interrupt
* controller and disconnecting the interrupt handler from the interrupt
* controller.
*
* If the device is in progress of transferring data on the SPI bus, this function
* returns a status indicating the device is busy. The user will be notified via
* the status handler when the transfer is complete, and at that time can again
* try to stop the device. As a master, we do not allow the device to be stopped
* while a transfer is in progress because the slave may be left in a bad state.
* As a slave, we do not allow the device to be stopped while a transfer is in
* progress because the master is not done with its transfer yet.
*
* @param    InstancePtr is a pointer to the XSpi instance to be worked on.
*
* @return
*
* XST_SUCCESS if the device is successfully started, or XST_DEVICE_BUSY if a
* transfer is in progress and cannot be stopped.
*
* @note
*
* This function makes use of internal resources that are shared between the
* XSpi_Stop() and XSpi_SetOptions() functions. So if one task might be setting
* device options options while another is trying to stop the device, the user
* is required to provide protection of this shared data (typically using a
* semaphore).
*
******************************************************************************/
XStatus XSpi_Stop(XSpi * InstancePtr)
{
	u16 ControlReg;

	XASSERT_NONVOID(InstancePtr != NULL);
	XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	/*
	 * Do not allow the user to stop the device while a transfer is in progress
	 */
	if (InstancePtr->IsBusy) {
		return XST_DEVICE_BUSY;
	}

	/*
	 * Disable the device. First disable the IPIF interrupts since there is a
	 * critical section here because this register is also modified during
	 * interrupt context. The device is likely disabled already since there is
	 * no transfer in progress, but we do it again just to be sure.
	 */
	XIIF_V123B_GINTR_DISABLE(InstancePtr->BaseAddr);

	ControlReg = XIo_In16(InstancePtr->BaseAddr + XSP_CR_OFFSET);
	XIo_Out16(InstancePtr->BaseAddr + XSP_CR_OFFSET,
		  ControlReg & ~XSP_CR_ENABLE_MASK);

	InstancePtr->IsStarted = 0;

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Resets the SPI device. Reset must only be called after the driver has been
* initialized. The configuration of the device after reset is the same as its
* configuration after initialization. Refer to the XSpi_Initialize function
* for more details. This is a hard reset of the device. Any data transfer that
* is in progress is aborted.
*
* The upper layer software is responsible for re-configuring (if necessary)
* and restarting the SPI device after the reset.
*
* @param    InstancePtr is a pointer to the XSpi instance to be worked on.
*
* @return
*
* None.
*
* @note
*
* None.
*
* @internal
*
* The reset is accomplished by setting the IPIF reset register. This takes
* care of resetting all hardware blocks, including the SPI device and its FIFOs.
*
******************************************************************************/
void XSpi_Reset(XSpi * InstancePtr)
{
	XASSERT_VOID(InstancePtr != NULL);
	XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	/*
	 * Abort any transfer that is in progress
	 */
	XSpi_Abort(InstancePtr);

	/* Reset any values that are not reset by the hardware reset such that
	 * the software state matches the hardware device
	 */
	InstancePtr->IsStarted = 0;
	InstancePtr->SlaveSelectReg = InstancePtr->SlaveSelectMask;

	/*
	 * Reset the entire IPIF at once
	 */
	XIIF_V123B_RESET(InstancePtr->BaseAddr);
}

/*****************************************************************************/
/**
*
* Transfers the specified data on the SPI bus. If the SPI device is configured
* to be a master, this function initiates bus communication and sends/receives
* the data to/from the selected SPI slave. If the SPI device is configured to
* be a slave, this function prepares the data to be sent/received when selected
* by a master. For every byte sent, a byte is received.
*
* The caller has the option of providing two different buffers for send and
* receive, or one buffer for both send and receive, or no buffer for receive.
* The receive buffer must be at least as big as the send buffer to prevent
* unwanted memory writes. This implies that the byte count passed in as an
* argument must be the smaller of the two buffers if they differ in size.
* Here are some sample usages:
* <pre>
*   XSpi_Transfer(InstancePtr, SendBuf, RecvBuf, ByteCount)
*       The caller wishes to send and receive, and provides two different
*       buffers for send and receive.
*
*   XSpi_Transfer(InstancePtr, SendBuf, NULL, ByteCount)
*       The caller wishes only to send and does not care about the received
*       data. The driver ignores the received data in this case.
*
*   XSpi_Transfer(InstancePtr, SendBuf, SendBuf, ByteCount)
*       The caller wishes to send and receive, but provides the same buffer
*       for doing both. The driver sends the data and overwrites the send
*       buffer with received data as it transfers the data.
*
*   XSpi_Transfer(InstancePtr, RecvBuf, RecvBuf, ByteCount)
*       The caller wishes to only receive and does not care about sending
*       data.  In this case, the caller must still provide a send buffer, but
*       it can be the same as the receive buffer if the caller does not care
*       what it sends.  The device must send N bytes of data if it wishes to
*       receive N bytes of data.
* </pre>
* Although this function takes a buffer as an argument, the driver can only
* transfer a limited number of bytes at time. It transfers only one byte at a
* time if there are no FIFOs, or it can transfer the number of bytes up to the
* size of the FIFO. A call to this function only starts the transfer, then
* subsequent transfer of the data is performed by the interrupt service routine
* until the entire buffer has been transferred. The status callback function is
* called when the entire buffer has been sent/received.
*
* This function is non-blocking. As a master, the SetSlaveSelect function must
* be called prior to this function.
*
* @param    InstancePtr is a pointer to the XSpi instance to be worked on.
* @param    SendBufPtr is a pointer to a buffer of data which is to be sent.
*           This buffer must not be NULL.
* @param    RecvBufPtr is a pointer to a buffer which will be filled with
*           received data. This argument can be NULL if the caller does not
*           wish to receive data.
* @param    ByteCount contains the number of bytes to send/receive. The number
*           of bytes received always equals the number of bytes sent.
*
* @return
*
* XST_SUCCESS if the buffers are successfully handed off to the driver
* for transfer. Otherwise, returns:
* - XST_DEVICE_IS_STOPPED if the device must be started before transferring data.
* - XST_DEVICE_BUSY indicates that a data transfer is already in progress.
*   This is determined by the driver.
* - XST_SPI_NO_SLAVE indicates the device is configured as a master and a
*   slave has not yet been selected.
*
* @notes
*
* This function is not thread-safe.  he higher layer software must ensure that
* no two threads are transferring data on the SPI bus at the same time.
*
******************************************************************************/
XStatus XSpi_Transfer(XSpi * InstancePtr, u8 * SendBufPtr,
		      u8 * RecvBufPtr, unsigned int ByteCount)
{
	u16 ControlReg;
	u8 StatusReg;
	u32 IntrEnable;

	/*
	 * The RecvBufPtr argument can be null
	 */
	XASSERT_NONVOID(InstancePtr != NULL);
	XASSERT_NONVOID(SendBufPtr != NULL);
	XASSERT_NONVOID(ByteCount > 0);
	XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	if (InstancePtr->IsStarted != XCOMPONENT_IS_STARTED) {
		return XST_DEVICE_IS_STOPPED;
	}

	/*
	 * Make sure there is not a transfer already in progress. No need to worry
	 * about a critical section here. Even if the Isr changes the busy flag
	 * just after we read it, a busy error is returned and the caller can retry
	 * when it gets the status handler callback indicating the transfer is done.
	 */
	if (InstancePtr->IsBusy) {
		return XST_DEVICE_BUSY;
	}

	/*
	 * Enter a critical section from here to the end of the function since
	 * state is modified, an interrupt is enabled, and the control register
	 * is modified (r/m/w).
	 */
	XIIF_V123B_GINTR_DISABLE(InstancePtr->BaseAddr);

	ControlReg = XIo_In16(InstancePtr->BaseAddr + XSP_CR_OFFSET);

	/*
	 * If configured as a master, be sure there is a slave select bit set
	 * in the slave select register. If no slaves have been selected, the
	 * value of the register will equal the mask.  When the device is in
	 * loopback mode, however, no slave selects need be set.
	 */
	if (ControlReg & XSP_CR_MASTER_MODE_MASK) {
		if ((ControlReg & XSP_CR_LOOPBACK_MASK) == 0) {
			if (InstancePtr->SlaveSelectReg ==
			    InstancePtr->SlaveSelectMask) {
				XIIF_V123B_GINTR_ENABLE(InstancePtr->BaseAddr);
				return XST_SPI_NO_SLAVE;
			}
		}
	}

	/*
	 * Set the slave select register to select the device on the SPI before
	 * starting the transfer of data
	 */
	XIo_Out32(InstancePtr->BaseAddr + XSP_SSR_OFFSET,
		  InstancePtr->SlaveSelectReg);
	/*
	 * Set the busy flag, which will be cleared in the Isr when the transfer
	 * is entirely done.
	 */
	InstancePtr->IsBusy = TRUE;

	/*
	 * Set up buffer pointers
	 */
	InstancePtr->SendBufferPtr = SendBufPtr;
	InstancePtr->RecvBufferPtr = RecvBufPtr;

	InstancePtr->RequestedBytes = ByteCount;
	InstancePtr->RemainingBytes = ByteCount;

	/*
	 * Fill the DTR/FIFO with as many bytes as it will take (or as many as we
	 * have to send). We use the full status bit to know if the device can take
	 * more data. By doing this, the driver does not need to know the size of
	 * the FIFO or that there even is a FIFO. The downside is that the status
	 * register must be read each loop iteration.
	 */
	StatusReg = XIo_In8(InstancePtr->BaseAddr + XSP_SR_OFFSET);

	while (((StatusReg & XSP_SR_TX_FULL_MASK) == 0) &&
	       (InstancePtr->RemainingBytes > 0)) {
		XIo_Out8(InstancePtr->BaseAddr + XSP_DTR_OFFSET,
			 *InstancePtr->SendBufferPtr);

		InstancePtr->SendBufferPtr++;
		InstancePtr->RemainingBytes--;

		StatusReg = XIo_In8(InstancePtr->BaseAddr + XSP_SR_OFFSET);
	}

	/*
	 * Enable the transmit empty interrupt, which we use to determine
	 * progress on the transmission.
	 */
	IntrEnable = XIIF_V123B_READ_IIER(InstancePtr->BaseAddr);
	XIIF_V123B_WRITE_IIER(InstancePtr->BaseAddr,
			      IntrEnable | XSP_INTR_TX_EMPTY_MASK);

	/*
	 * Start the transfer by no longer inhibiting the transmitter and enabling
	 * the device. For a master, this will in fact start the transfer, but for
	 * a slave it only prepares the device for a transfer that must be initiated
	 * by a master.
	 */
	ControlReg &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XIo_Out16(InstancePtr->BaseAddr + XSP_CR_OFFSET, ControlReg);

	/*
	 * End critical section
	 */
	XIIF_V123B_GINTR_ENABLE(InstancePtr->BaseAddr);

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Selects or deselect the slave with which the master communicates. Each slave
* that can be selected is represented in the slave select register by a bit.
* The argument passed to this function is the bit mask with a 1 in the bit
* position of the slave being selected. Only one slave can be selected.
*
* The user is not allowed to deselect the slave while a transfer is in progress.
* If no transfer is in progress, the user can select a new slave, which
* implicitly deselects the current slave. In order to explicitly deselect the
* current slave, a zero can be passed in as the argument to the function.
*
* @param    InstancePtr is a pointer to the XSpi instance to be worked on.
* @param    SlaveMask is a 32-bit mask with a 1 in the bit position of the slave
*           being selected. Only one slave can be selected. The SlaveMask can
*           be zero if the slave is being deselected.
*
* @return
*
* XST_SUCCESS if the slave is selected or deselected successfully.
* Otherwise, returns:
* - XST_DEVICE_BUSY if a transfer is in progress, slave cannot be changed
* - XST_SPI_TOO_MANY_SLAVES if more than one slave is being selected.
*
* @note
*
* This function only sets the slave which will be selected when a transfer
* occurs. The slave is not selected when the SPI is idle. The slave select
* has no affect when the device is configured as a slave.
*
******************************************************************************/
XStatus XSpi_SetSlaveSelect(XSpi * InstancePtr, u32 SlaveMask)
{
	int NumAsserted;
	int Index;

	XASSERT_NONVOID(InstancePtr != NULL);
	XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	/*
	 * Do not allow the slave select to change while a transfer is in progress.
	 * No need to worry about a critical section here since even if the Isr
	 * changes the busy flag just after we read it, the function will return
	 * busy and the caller can retry when notified that their current transfer
	 * is done.
	 */
	if (InstancePtr->IsBusy) {
		return XST_DEVICE_BUSY;
	}

	/*
	 * Verify that only one bit in the incoming slave mask is set
	 */
	NumAsserted = 0;
	for (Index = (InstancePtr->NumSlaveBits - 1); Index >= 0; Index--) {
		if ((SlaveMask >> Index) & 0x1) {
			/* this bit is asserted */
			NumAsserted++;
		}
	}

	/*
	 * Return an error if more than one slave is selected
	 */
	if (NumAsserted > 1) {
		return XST_SPI_TOO_MANY_SLAVES;
	}

	/*
	 * A single slave is either being selected or the incoming SlaveMask is
	 * zero, which means the slave is being deselected. Setup the value to be
	 * written to the slave select register as the inverse of the slave mask.
	 */
	InstancePtr->SlaveSelectReg = ~SlaveMask;

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Gets the current slave select bit mask for the SPI device.
*
* @param    InstancePtr is a pointer to the XSpi instance to be worked on.
*
* @return
*
* The value returned is a 32-bit mask with a 1 in the bit position of the slave
* currently selected. The value may be zero if no slaves are selected.
*
* @note
*
* None.
*
******************************************************************************/
u32 XSpi_GetSlaveSelect(XSpi * InstancePtr)
{
	u32 SsReg;

	XASSERT_NONVOID(InstancePtr != NULL);
	XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	SsReg = XIo_In32(InstancePtr->BaseAddr + XSP_SSR_OFFSET);

	/*
	 * Return the inverse of the hardware register and mask it so unimplemented
	 * bits do not show up as ones
	 */
	return ~InstancePtr->SlaveSelectReg;
}

/*****************************************************************************/
/**
*
* Sets the status callback function, the status handler, which the driver calls
* when it encounters conditions that should be reported to the higher layer
* software. The handler executes in an interrupt context, so it must minimize
* the amount of processing performed such as transferring data to a thread
* context. One of the following status events is passed to the status handler.
* <pre>
*   XST_SPI_MODE_FAULT          A mode fault error occurred, meaning another
*                               master tried to select this device as a slave
*                               when this device was configured to be a master.
*                               Any transfer in progress is aborted.
*
*   XST_SPI_TRANSFER_DONE       The requested data transfer is done
*
*   XST_SPI_TRANSMIT_UNDERRUN   As a slave device, the master clocked data
*                               but there were none available in the transmit
*                               register/FIFO. This typically means the slave
*                               application did not issue a transfer request
*                               fast enough, or the processor/driver could not
*                               fill the transmit register/FIFO fast enough.
*
*   XST_SPI_RECEIVE_OVERRUN     The SPI device lost data. Data was received
*                               but the receive data register/FIFO was full.
*                               This indicates that the device is receiving data
*
*                               faster than the processor/driver can consume it.
*   XST_SPI_SLAVE_MODE_FAULT    A slave SPI device was selected as a slave while
*                               it was disabled.  This indicates the master is
*                               already transferring data (which is being
*                               dropped until the slave application issues a
*                               transfer).
* </pre>
* @param    InstancePtr is a pointer to the XSpi instance to be worked on.
* @param    CallBackRef is the upper layer callback reference passed back
*           when the callback function is invoked.
* @param    FuncPtr is the pointer to the callback function.
*
* @return
*
* None.
*
* @note
*
* The handler is called within interrupt context, so it should do its work
* quickly and queue potentially time-consuming work to a task-level thread.
*
******************************************************************************/
void XSpi_SetStatusHandler(XSpi * InstancePtr, void *CallBackRef,
			   XSpi_StatusHandler FuncPtr)
{
	XASSERT_VOID(InstancePtr != NULL);
	XASSERT_VOID(FuncPtr != NULL);
	XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	InstancePtr->StatusHandler = FuncPtr;
	InstancePtr->StatusRef = CallBackRef;
}

/*****************************************************************************/
/**
*
* This is a stub for the status callback. The stub is here in case the upper
* layers forget to set the handler.
*
* @param    CallBackRef is a pointer to the upper layer callback reference
* @param    StatusEvent is the event that just occurred.
* @param    ByteCount is the number of bytes transferred up until the event
*           occurred.
*
* @return
*
* None.
*
* @note
*
* None.
*
******************************************************************************/
static void StubStatusHandler(void *CallBackRef, XStatus StatusEvent,
			      unsigned int ByteCount)
{
	XASSERT_VOID_ALWAYS();
}

/*****************************************************************************/
/**
*
* The interrupt handler for SPI interrupts. This function must be connected
* by the user to an interrupt source. This function does not save and restore
* the processor context such that the user must provide this processing.
*
* The interrupts that are handled are:
*
* - Mode Fault Error. This interrupt is generated if this device is selected
*   as a slave when it is configured as a master. The driver aborts any data
*   transfer that is in progress by resetting FIFOs (if present) and resetting
*   its buffer pointers. The upper layer software is informed of the error.
*
* - Data Transmit Register (FIFO) Empty. This interrupt is generated when the
*   transmit register or FIFO is empty. The driver uses this interrupt during a
*   transmission to continually send/receive data until there is no more data
*   to send/receive.
*
* - Data Transmit Register (FIFO) Underrun. This interrupt is generated when
*   the SPI device, when configured as a slave, attempts to read an empty
*   DTR/FIFO.  An empty DTR/FIFO usually means that software is not giving the
*   device data in a timely manner. No action is taken by the driver other than
*   to inform the upper layer software of the error.
*
* - Data Receive Register (FIFO) Overrun. This interrupt is generated when the
*   SPI device attempts to write a received byte to an already full DRR/FIFO.
*   A full DRR/FIFO usually means software is not emptying the data in a timely
*   manner.  No action is taken by the driver other than to inform the upper
*   layer software of the error.
*
* - Slave Mode Fault Error. This interrupt is generated if a slave device is
*   selected as a slave while it is disabled. No action is taken by the driver
*   other than to inform the upper layer software of the error.
*
* @param    InstancePtr is a pointer to the XSpi instance to be worked on.
*
* @return
*
* None.
*
* @note
*
* The slave select register is being set to deselect the slave when a transfer
* is complete.  This is being done regardless of whether it is a slave or a
* master since the hardware does not drive the slave select as a slave.
*
******************************************************************************/
void XSpi_InterruptHandler(void *InstancePtr)
{
	XSpi *SpiPtr = (XSpi *) InstancePtr;
	u32 IntrStatus;
	unsigned int BytesDone;	/* number of bytes done so far */

	XASSERT_VOID(InstancePtr != NULL);

	/*
	 * Update the stats for the number of interrupts
	 */
	SpiPtr->Stats.NumInterrupts++;

	/*
	 * Get the IPIF IP interrupts. Immediately clear the interrupts in case
	 * this Isr causes another interrupt to be generated. If we clear at the
	 * end of the Isr, we may miss this newly generated interrupt. This occurs
	 * because we transmit from within the Isr, potentially causing another
	 * TX_EMPTY interrupt.
	 */
	IntrStatus = XIIF_V123B_READ_IISR(SpiPtr->BaseAddr);
	XIIF_V123B_WRITE_IISR(SpiPtr->BaseAddr, IntrStatus);

	/*
	 * Check for mode fault error. We want to check for this error first,
	 * before checking for progress of a transfer, since this error needs
	 * to abort any operation in progress.
	 */
	if (IntrStatus & XSP_INTR_MODE_FAULT_MASK) {
		BytesDone = SpiPtr->RequestedBytes - SpiPtr->RemainingBytes;
		SpiPtr->Stats.ModeFaults++;

		/*
		 * Abort any operation currently in progress. This includes clearing
		 * the mode fault condition by reading the status register. Note that
		 * the status register should be read after the Abort since reading
		 * the status register clears the mode fault condition and would
		 * cause the device to restart any transfer that may be in progress.
		 */
		XSpi_Abort(SpiPtr);

		(void)XIo_In8(SpiPtr->BaseAddr + XSP_SR_OFFSET);

		SpiPtr->StatusHandler(SpiPtr->StatusRef, XST_SPI_MODE_FAULT,
				      BytesDone);

		return;		/* do not continue servicing other interrupts */
	}

	if (IntrStatus & XSP_INTR_TX_EMPTY_MASK) {
		u16 ControlReg;
		u8 StatusReg;
		u8 TempData;

		/*
		 * A transmit has just completed. Process received data and check
		 * for more data to transmit. Always inhibit the transmitter while the
		 * Isr re-fills the transmit register/FIFO, or make sure it is stopped
		 * if we're done.
		 */
		ControlReg = XIo_In16(SpiPtr->BaseAddr + XSP_CR_OFFSET);
		XIo_Out16(SpiPtr->BaseAddr + XSP_CR_OFFSET,
			  ControlReg | XSP_CR_TRANS_INHIBIT_MASK);

		/*
		 * First get the data received as a result of the transmit that just
		 * completed.  We get all the data available by reading the status
		 * register to determine when the receive register/FIFO is empty. Always
		 * get the received data, but only fill the receive buffer if it points
		 * to something (the upper layer software may not care to receive data).
		 */
		StatusReg = XIo_In8(SpiPtr->BaseAddr + XSP_SR_OFFSET);
		while ((StatusReg & XSP_SR_RX_EMPTY_MASK) == 0) {
			SpiPtr->Stats.BytesTransferred++;

			TempData = XIo_In8(SpiPtr->BaseAddr + XSP_DRR_OFFSET);
			if (SpiPtr->RecvBufferPtr != NULL) {
				*SpiPtr->RecvBufferPtr++ = TempData;
			}

			StatusReg = XIo_In8(SpiPtr->BaseAddr + XSP_SR_OFFSET);
		}

		/*
		 * See if there is more data to send
		 */
		if (SpiPtr->RemainingBytes > 0) {
			/*
			 * Fill the DTR/FIFO with as many bytes as it will take (or as many
			 * as we have to send). We use the full status bit to know if the
			 * device can take more data. By doing this, the driver does not
			 * need to know the size of the FIFO or that there even is a FIFO.
			 * The downside is that the status must be read each loop iteration.
			 */
			StatusReg = XIo_In8(SpiPtr->BaseAddr + XSP_SR_OFFSET);
			while (((StatusReg & XSP_SR_TX_FULL_MASK) == 0) &&
			       (SpiPtr->RemainingBytes > 0)) {
				XIo_Out8(SpiPtr->BaseAddr + XSP_DTR_OFFSET,
					 *SpiPtr->SendBufferPtr);

				SpiPtr->SendBufferPtr++;
				SpiPtr->RemainingBytes--;

				StatusReg =
				    XIo_In8(SpiPtr->BaseAddr + XSP_SR_OFFSET);
			}

			/*
			 * Start the transfer by not inhibiting the transmitter any longer
			 */
			XIo_Out16(SpiPtr->BaseAddr + XSP_CR_OFFSET, ControlReg);
		} else {
			u32 IntrEnable;

			/*
			 * Select the slave on the SPI bus when the transfer is complete,
			 * this is necessary for some SPI devices, such as serial EEPROMs
			 * work correctly as chip enable may be connected to slave select
			 */
			XIo_Out32(SpiPtr->BaseAddr + XSP_SSR_OFFSET,
				  SpiPtr->SlaveSelectMask);
			/*
			 * No more data to send.  Disable the interrupt and inform the
			 * upper layer software that the transfer is done. The interrupt
			 * will be re-enabled when another transfer is initiated.
			 */
			IntrEnable = XIIF_V123B_READ_IIER(SpiPtr->BaseAddr);
			XIIF_V123B_WRITE_IIER(SpiPtr->BaseAddr,
					      IntrEnable &
					      ~XSP_INTR_TX_EMPTY_MASK);

			SpiPtr->IsBusy = FALSE;

			SpiPtr->StatusHandler(SpiPtr->StatusRef,
					      XST_SPI_TRANSFER_DONE,
					      SpiPtr->RequestedBytes);
		}
	}

	/*
	 * Check for slave mode fault. Simply report the error and bump stats.
	 */
	if (IntrStatus & XSP_INTR_SLAVE_MODE_FAULT_MASK) {
		BytesDone = SpiPtr->RequestedBytes - SpiPtr->RemainingBytes;
		SpiPtr->Stats.SlaveModeFaults++;
		SpiPtr->StatusHandler(SpiPtr->StatusRef,
				      XST_SPI_SLAVE_MODE_FAULT, BytesDone);
	}

	/*
	 * Check for overrun and underrun errors, bump stats
	 */
	if (IntrStatus & XSP_INTR_RX_OVERRUN_MASK) {
		BytesDone = SpiPtr->RequestedBytes - SpiPtr->RemainingBytes;
		SpiPtr->Stats.RecvOverruns++;
		SpiPtr->StatusHandler(SpiPtr->StatusRef,
				      XST_SPI_RECEIVE_OVERRUN, BytesDone);
	}

	if (IntrStatus & XSP_INTR_TX_UNDERRUN_MASK) {
		BytesDone = SpiPtr->RequestedBytes - SpiPtr->RemainingBytes;
		SpiPtr->Stats.XmitUnderruns++;
		SpiPtr->StatusHandler(SpiPtr->StatusRef,
				      XST_SPI_TRANSMIT_UNDERRUN, BytesDone);
	}
}

/*****************************************************************************/
/**
*
* Aborts a transfer in progress by setting the stop bit in the control register,
* then resetting the FIFOs if present. The byte counts are cleared and the
* busy flag is set to false.
*
* @param    InstancePtr is a pointer to the XSpi instance to be worked on.
*
* @return
*
* None.
*
* @note
*
* This function does a read/modify/write of the control register. The user of
* this function needs to take care of critical sections.
*
******************************************************************************/
void XSpi_Abort(XSpi * InstancePtr)
{
	u16 ControlReg;

	/*
	 * Deselect the slave on the SPI bus to abort a transfer, this must be
	 * done before the device is disabled such that the signals which are
	 * driven by the device are changed without the device enabled
	 */
	XIo_Out32(InstancePtr->BaseAddr + XSP_SSR_OFFSET,
		  InstancePtr->SlaveSelectMask);
	/*
	 * Abort the operation currently in progress. Clear the mode
	 * fault condition by reading the status register (done) then
	 * writing the control register.
	 */
	ControlReg = XIo_In16(InstancePtr->BaseAddr + XSP_CR_OFFSET);

	/*
	 * Stop any transmit in progress and reset the FIFOs if they exist, don't
	 * disable the device just inhibit any data from being sent
	 */
	ControlReg |= XSP_CR_TRANS_INHIBIT_MASK;

	if (InstancePtr->HasFifos) {
		ControlReg |= (XSP_CR_TXFIFO_RESET_MASK |
			       XSP_CR_RXFIFO_RESET_MASK);
	}

	XIo_Out16(InstancePtr->BaseAddr + XSP_CR_OFFSET, ControlReg);

	InstancePtr->RemainingBytes = 0;
	InstancePtr->RequestedBytes = 0;
	InstancePtr->IsBusy = FALSE;
}

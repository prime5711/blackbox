#include "ipipe_para.h"
#include <asm-arm/arch-davinci/dm355_ipipe_hw.h>
#include <asm-arm/arch-davinci/dm355_ipipe.h>
#include <linux/config.h>

#define  WIDTH_I 624
#define  HEIGHT_I 456
#define  WIDTH_O 640
#define  HEIGHT_O 480

#if 0
static int yeeTable[] = {
  #include "edgeEnhanceTable.h"
};
static int NoiseFilterTHR[] = {
  #include "NoiseFiltThr.h"
};
static int NoiseFilterSTR[] = {
  #include "NoiseFiltStr.h"
};
static int GammaTableall[] = {
 #include "gammaTable.h"
};
#endif
struct ipipe_params param_def = {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS13_0,
				.clock_select = SDRAM_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_SIXTH,//DIVIDE_HALF,
				.source = SDRAM_RAW,
				.decimation = DECIMATION_OFF,
				.mode = ONE_SHOT,

				.glob_hor_size = WIDTH_I + 8,//632,
				.glob_ver_size = HEIGHT_I + 10,//466,
				.hnum = WIDTH_I,//624,
				.vnum = HEIGHT_I,//456,
				.adofs = WIDTH_I * 2,/*1248,  to make it
							32 alligned */
				.rsz = 16,   /* resize ratio = 16/rsz:
                                                valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
	.ipipe_colpat_elep = GREEN_BLUE,
        .ipipe_colpat_elop = BLUE,
        .ipipe_colpat_olep = RED,
        .ipipe_colpat_olop = GREEN_RED,

	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 1,
	.ipipe_vsz = HEIGHT_I - 1,//456 - 1,
	.ipipe_hst = 0,//check
	.ipipe_hsz =  WIDTH_I - 1,//624 - 1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = ENABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 512,//512,//1023,//256,
		        .wb2_wg_r = 444,
		        .wb2_wg_gr= 256,
		        .wb2_wg_gb= 256,
		        .wb2_wg_b = 568//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_BYPASS,
		         .gmm_cfg_bypg = GC_BYPASS,
			 .gmm_cfg_bypb = GC_BYPASS,
		         .gmm_cfg_tbl  = IPIPE_RAM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = ENABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = DISABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = ONE_SHOT,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
				 .rsz_o_vsz = HEIGHT_O - 1,
			         .rsz_o_hsz = WIDTH_O - 1,
				 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 243,
				 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 243,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         ONE_SHOT,
				 0,
				 0,
			         0,
			         239,
				 319,
			         0,
			         0,
			        256,
			        0,
				256,
			        CUBIC,
			        INTERNAL_VALUE,
			        0
			    }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				DISABLE,
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = WIDTH_O * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   },
		           {
			       	0,
			       	0,
			       	0,
			       	0,
		           	WIDTH_O * 2,
			        0,
			        8191
   			    }
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = DISABLE
};


struct ipipe_params ipipe_mt9p031_720P= {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS11_0,
				.clock_select = SDRAM_CLK,//PIXCEL_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_FOURTH,//DIVIDE_HALF,
				.source = SDRAM_RAW,
				.decimation = DECIMATION_OFF,
				.mode = CONTINUOUS,//ONE_SHOT,

				.glob_hor_size = 1280+8,
				.glob_ver_size = 720+10,
				.hnum = 1280,
				.vnum = 720,
				.adofs = 1280 * 2,/*1248,  to make it 32 alligned */
				.rsz = ONE,   /* resize ratio = 16/rsz:  valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = CONTINUOUS,//ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,
#if 0
    .ipipe_colpat_elep = GREEN_RED,           .ipipe_colpat_elop = RED,
    .ipipe_colpat_olep = BLUE,    .ipipe_colpat_olop = GREEN_BLUE,

    .ipipe_colpat_elep = RED,           .ipipe_colpat_elop = GREEN_RED,
    .ipipe_colpat_olep = GREEN_BLUE,    .ipipe_colpat_olop = BLUE,

    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,

    .ipipe_colpat_elep = GREEN_BLUE,           .ipipe_colpat_elop = BLUE,
    .ipipe_colpat_olep = RED,    .ipipe_colpat_olop = GREEN_RED,
#endif
	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 1,
	.ipipe_vsz = 720-1,
	.ipipe_hst = 0,
	.ipipe_hsz = 1280-1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = DISABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 0x200,//512,//1023,//256,
		        .wb2_wg_r = 0xA0,
		        .wb2_wg_gr= 0x80,
		        .wb2_wg_gb= 0x80,
		        .wb2_wg_b = 0xA8//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_ENABLE,
		         .gmm_cfg_bypg = GC_ENABLE,
			 .gmm_cfg_bypb = GC_ENABLE,
		         .gmm_cfg_tbl  = IPIPE_ROM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = DISABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = ENABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 720-1,
			         .rsz_o_hsz = 1280-1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 256,
					//unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 256,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
				 .rsz_o_vsz = 192-1,//360-1,
			         .rsz_o_hsz = 352-1,//640-1,
				 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 930,//512,
					//unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 930,//512,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				.rsz_rgb_en = DISABLE,//....check
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 1280 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 720
   			   },
		        {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 352*2,//640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 192,//360
   			   },
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = ENABLE
};

struct ipipe_params ipipe_mt9p031_SXGA= {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS11_0,
				.clock_select = SDRAM_CLK,//PIXCEL_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_FOURTH,//DIVIDE_HALF,
				.source = SDRAM_RAW,
				.decimation = DECIMATION_OFF,
				.mode = CONTINUOUS,//ONE_SHOT,

				.glob_hor_size = 1280 + 8,
				.glob_ver_size = 960 + 10,
				.hnum = 1280,
				.vnum = 960,
				.adofs = 1280 * 2,/*1248,  to make it 32 alligned */
				.rsz = ONE,   /* resize ratio = 16/rsz:  valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = CONTINUOUS,//ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,
	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 1,
	.ipipe_vsz = 960 - 1,
	.ipipe_hst = 0,
	.ipipe_hsz = 1280 - 1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = DISABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 0x200,//512,//1023,//256,
		        .wb2_wg_r = 0xA0,
		        .wb2_wg_gr= 0x80,
		        .wb2_wg_gb= 0x80,
		        .wb2_wg_b = 0xA8//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_ENABLE,
		         .gmm_cfg_bypg = GC_ENABLE,
			 .gmm_cfg_bypb = GC_ENABLE,
		         .gmm_cfg_tbl  = IPIPE_ROM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = DISABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = ENABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 1024-1,
			         .rsz_o_hsz = 1280-1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 240,
					//unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 256,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
				 .rsz_o_vsz = 240-1,//360-1,
			         .rsz_o_hsz = 320-1,//640-1,
				 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 1024,//512,
					//unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 1024,//512,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				.rsz_rgb_en = DISABLE,//....check
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 1280 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 1024
   			   },
		        {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 320*2,//640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 240,//360
   			   },
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = ENABLE
};

struct ipipe_params ipipe_mt9p031_SXVGA= {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS11_0,
				.clock_select = SDRAM_CLK,//PIXCEL_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_FOURTH,//DIVIDE_HALF,
				.source = SDRAM_RAW,
				.decimation = DECIMATION_OFF,
				.mode = CONTINUOUS,//ONE_SHOT,

				.glob_hor_size = 1280 + 8,
				.glob_ver_size = 960 + 10,
				.hnum = 1280,
				.vnum = 960,
				.adofs = 1280 * 2,/*1248,  to make it 32 alligned */
				.rsz = ONE,   /* resize ratio = 16/rsz:  valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = CONTINUOUS,//ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,
	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 1,
	.ipipe_vsz = 960 - 1,
	.ipipe_hst = 0,
	.ipipe_hsz = 1280 - 1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = DISABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 0x200,//512,//1023,//256,
		        .wb2_wg_r = 0xA0,
		        .wb2_wg_gr= 0x80,
		        .wb2_wg_gb= 0x80,
		        .wb2_wg_b = 0xA8//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_ENABLE,
		         .gmm_cfg_bypg = GC_ENABLE,
			 .gmm_cfg_bypb = GC_ENABLE,
		         .gmm_cfg_tbl  = IPIPE_ROM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = DISABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = ENABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 960-1,
			         .rsz_o_hsz = 1280-1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 256,
					//unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 256,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
				 .rsz_o_vsz = 330-1,//360-1,
			         .rsz_o_hsz = 440-1,//640-1,
				 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 745,//512,
					//unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 745,//512,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				.rsz_rgb_en = DISABLE,//....check
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 1280 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 960
   			   },
		        {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 440*2,//640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 330,//360
   			   },
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = ENABLE
};

struct ipipe_params ipipe_mt9p031_VGA_VGA= {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS11_0,
				.clock_select = SDRAM_CLK,//PIXCEL_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_FOURTH,//DIVIDE_HALF,
				.source = SDRAM_RAW,
				.decimation = DECIMATION_OFF,
				.mode = CONTINUOUS,//ONE_SHOT,

				.glob_hor_size = 1280 + 8,
				.glob_ver_size = 960 + 10,
				.hnum = 1280,
				.vnum = 960,
				.adofs = 1280 * 2,/*1248,  to make it 32 alligned */
				.rsz = ONE,   /* resize ratio = 16/rsz:  valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = CONTINUOUS,//ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,
	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 1,
	.ipipe_vsz = 960 - 1,
	.ipipe_hst = 0,
	.ipipe_hsz = 1280 - 1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = DISABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 0x200,//512,//1023,//256,
		        .wb2_wg_r = 0xA0,
		        .wb2_wg_gr= 0x80,
		        .wb2_wg_gb= 0x80,
		        .wb2_wg_b = 0xA8//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_ENABLE,
		         .gmm_cfg_bypg = GC_ENABLE,
			 .gmm_cfg_bypb = GC_ENABLE,
		         .gmm_cfg_tbl  = IPIPE_ROM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = DISABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = ENABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 600-1,
			         .rsz_o_hsz = 800-1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 410,
					//unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 410,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
				     .rsz_o_vsz = 480-1,//360-1,
			         .rsz_o_hsz = 640-1,//640-1,
				     .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 512,//512,
					//unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 512,//512,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				.rsz_rgb_en = DISABLE,//....check
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 800 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 600
   			   },
		        {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640*2,//640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480,//360
   			   },
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = ENABLE
};

struct ipipe_params ipipe_mt9p031_VGA_30fps= {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS11_0,
				.clock_select = SDRAM_CLK,//PIXCEL_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_FOURTH,//DIVIDE_HALF,
				.source = SDRAM_RAW,
				.decimation = DECIMATION_OFF,
				.mode = CONTINUOUS,//ONE_SHOT,

				.glob_hor_size = 1280 + 8,
				.glob_ver_size = 960 + 10,
				.hnum = 1280,
				.vnum = 960,
				.adofs = 1280 * 2,/*1248,  to make it 32 alligned */
				.rsz = ONE,   /* resize ratio = 16/rsz:  valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = CONTINUOUS,//ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,
	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 1,
	.ipipe_vsz = 960 - 1,
	.ipipe_hst = 0,
	.ipipe_hsz = 1280 - 1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = DISABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 0x200,//512,//1023,//256,
		        .wb2_wg_r = 0xA0,
		        .wb2_wg_gr= 0x80,
		        .wb2_wg_gb= 0x80,
		        .wb2_wg_b = 0xA8//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_ENABLE,
		         .gmm_cfg_bypg = GC_ENABLE,
			 .gmm_cfg_bypb = GC_ENABLE,
		         .gmm_cfg_tbl  = IPIPE_ROM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = DISABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = ENABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 480 - 1,
			         .rsz_o_hsz = 640 - 1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 512,
					 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 512,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 480 - 1,
			         .rsz_o_hsz = 640 - 1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 512,
					 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 512,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				.rsz_rgb_en = DISABLE,//....check
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   },
		        {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   }
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = ENABLE
};

struct ipipe_params ipipe_mt9p031_VGA_BIN_30fps= {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS11_0,
				.clock_select = SDRAM_CLK,//PIXCEL_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_FOURTH,//DIVIDE_HALF,
				.source = SDRAM_RAW,
				.decimation = DECIMATION_OFF,
				.mode = CONTINUOUS,//ONE_SHOT,

				.glob_hor_size = 640 + 8,
				.glob_ver_size = 480 + 10,
				.hnum = 640,
				.vnum = 480,
				.adofs = 640 * 2,/*1248,  to make it 32 alligned */
				.rsz = ONE,   /* resize ratio = 16/rsz:  valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = CONTINUOUS,//ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,
	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 1,
	.ipipe_vsz = 480 - 1,
	.ipipe_hst = 0,
	.ipipe_hsz = 640 - 1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = DISABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 0x200,//512,//1023,//256,
		        .wb2_wg_r = 0xA0,
		        .wb2_wg_gr= 0x80,
		        .wb2_wg_gb= 0x80,
		        .wb2_wg_b = 0xA8//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_ENABLE,
		         .gmm_cfg_bypg = GC_ENABLE,
			 .gmm_cfg_bypb = GC_ENABLE,
		         .gmm_cfg_tbl  = IPIPE_ROM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = DISABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = ENABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
				 .rsz_o_vsz = 480 - 1,
			         .rsz_o_hsz = 640 - 1,
				 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 256,
					 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 256,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
				 .rsz_o_vsz = 480 - 1,
			         .rsz_o_hsz = 640 - 1,
				 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 256,
					 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 256,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				.rsz_rgb_en = DISABLE,//....check
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   },
		        {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   }
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = ENABLE
};


struct ipipe_params ipipe_mt9p031_VGA_60fps= {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS11_0,
				.clock_select = SDRAM_CLK,//PIXCEL_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_FOURTH,//DIVIDE_HALF,
				.source = SDRAM_RAW,
				.decimation = DECIMATION_OFF,
				.mode = CONTINUOUS,//ONE_SHOT,

				.glob_hor_size = 1280 + 8,
				.glob_ver_size = 960 + 10,
				.hnum = 1280,
				.vnum = 960,
				.adofs = 1280 * 2,/*1248,  to make it 32 alligned */
				.rsz = ONE,   /* resize ratio = 16/rsz:  valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = CONTINUOUS,//ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,
	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 13,
	.ipipe_vsz = 960 - 1,
	.ipipe_hst = 4,
	.ipipe_hsz = 1280 - 1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = DISABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 1023,//512,//1023,//256,
		        .wb2_wg_r = 0xA0,
		        .wb2_wg_gr= 0x80,
		        .wb2_wg_gb= 0x80,
		        .wb2_wg_b = 0xA8//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_ENABLE,
		         .gmm_cfg_bypg = GC_ENABLE,
			 .gmm_cfg_bypb = GC_ENABLE,
		         .gmm_cfg_tbl  = IPIPE_ROM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = DISABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = ENABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 480 - 1,
			         .rsz_o_hsz = 640 - 1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 512,
					 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 512,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 480 - 1,
			         .rsz_o_hsz = 640 - 1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 512,
					 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 512,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				.rsz_rgb_en = DISABLE,//....check
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   },
		        {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   }
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = ENABLE
};


struct ipipe_params ipipe_mt9t001_VGA_60fps= {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS11_0,
				.clock_select = PIXCEL_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_FOURTH,//DIVIDE_HALF,
				.source = CCDC,
				.decimation = DECIMATION_OFF,
				.mode = CONTINUOUS,//ONE_SHOT,

				.glob_hor_size = 640 + 8,
				.glob_ver_size = 480 + 10,
				.hnum = 640,
				.vnum = 480,
				.adofs = 640 * 2,/*1248,  to make it 32 alligned */
				.rsz = ONE,   /* resize ratio = 16/rsz:  valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = CONTINUOUS,//ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,
	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 9,
	.ipipe_vsz = 480 - 1,
	.ipipe_hst = 10,
	.ipipe_hsz = 640 - 1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = DISABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 1023,//512,//1023,//256,
		        .wb2_wg_r = 0xA0,
		        .wb2_wg_gr= 0x80,
		        .wb2_wg_gb= 0x80,
		        .wb2_wg_b = 0xA8//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_ENABLE,
		         .gmm_cfg_bypg = GC_ENABLE,
			 .gmm_cfg_bypb = GC_ENABLE,
		         .gmm_cfg_tbl  = IPIPE_ROM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = DISABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = ENABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 480 - 1,
			         .rsz_o_hsz = 640 - 1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 256,
					 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 256,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 480 - 1,
			         .rsz_o_hsz = 640 - 1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 256,
					 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 256,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				.rsz_rgb_en = DISABLE,//....check
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   },
		        {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   }
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = ENABLE
};

struct ipipe_params ipipe_mt9t001_VGA_30fps= {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS11_0,
				.clock_select = PIXCEL_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_FOURTH,//DIVIDE_HALF,
				.source = CCDC,
				.decimation = DECIMATION_OFF,
				.mode = CONTINUOUS,//ONE_SHOT,

				.glob_hor_size = 640 + 8,
				.glob_ver_size = 480 + 10,
				.hnum = 640,
				.vnum = 480,
				.adofs = 640 * 2,/*1248,  to make it 32 alligned */
				.rsz = ONE,   /* resize ratio = 16/rsz:  valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = CONTINUOUS,//ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,
	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 9,
	.ipipe_vsz = 480 - 1,
	.ipipe_hst = 10,
	.ipipe_hsz = 640 - 1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = DISABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 0x200,//512,//1023,//256,
		        .wb2_wg_r = 0xA0,
		        .wb2_wg_gr= 0x80,
		        .wb2_wg_gb= 0x80,
		        .wb2_wg_b = 0xA8//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_ENABLE,
		         .gmm_cfg_bypg = GC_ENABLE,
			 .gmm_cfg_bypb = GC_ENABLE,
		         .gmm_cfg_tbl  = IPIPE_ROM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = DISABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = ENABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 480 - 1,
			         .rsz_o_hsz = 640 - 1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 256,
					 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 256,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 480 - 1,
			         .rsz_o_hsz = 640 - 1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 256,
					 //unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 256,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				.rsz_rgb_en = DISABLE,//....check
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   },
		        {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 480
   			   }
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = ENABLE
};

struct ipipe_params ipipe_mt9t001_720P= {
	.ipipeif_param = {
				/*IPPEIF config register*/
				.data_shift = BITS11_0,
				.clock_select = PIXCEL_CLK,

				.ialaw = ALAW_OFF,
				.pack_mode = SIXTEEN_BIT,
				.avg_filter = AVG_OFF,
				.clk_div = DIVIDE_FOURTH,//DIVIDE_HALF,
				.source = CCDC,
				.decimation = DECIMATION_OFF,
				.mode = CONTINUOUS,//ONE_SHOT,

				.glob_hor_size = 1280+8,
				.glob_ver_size = 720+10,
				.hnum = 1280,
				.vnum = 720,
				.adofs = 1280 * 2,/*1248,  to make it 32 alligned */
				.rsz = ONE,   /* resize ratio = 16/rsz:  valid range (16-112) */
#if defined( CONFIG_MACH_DAVINCI_DM355_IPNC )
				.gain = 0x200    /* (precision is U10Q9) */
#else
				.gain = 0x3FF    /* (precision is U10Q9) */
#endif
			},
	.ipipe_mode = CONTINUOUS,//ONE_SHOT,
	/*input/output datapath register*/
	.ipipe_dpaths_fmt = RAW2YUV,
	.ipipe_dpaths_bypass = RAW_MODE_OFF,//...check

	/*color pattern register*/
    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,
#if 0
    .ipipe_colpat_elep = GREEN_RED,           .ipipe_colpat_elop = RED,
    .ipipe_colpat_olep = BLUE,    .ipipe_colpat_olop = GREEN_BLUE,

    .ipipe_colpat_elep = RED,           .ipipe_colpat_elop = GREEN_RED,
    .ipipe_colpat_olep = GREEN_BLUE,    .ipipe_colpat_olop = BLUE,

    .ipipe_colpat_elep = BLUE,           .ipipe_colpat_elop = GREEN_BLUE,
    .ipipe_colpat_olep = GREEN_RED,    .ipipe_colpat_olop = RED,

    .ipipe_colpat_elep = GREEN_BLUE,           .ipipe_colpat_elop = BLUE,
    .ipipe_colpat_olep = RED,    .ipipe_colpat_olop = GREEN_RED,
#endif
	/*horizontal/vertical start, horizontal/vertical size*/
	.ipipe_vst = 13,
	.ipipe_vsz = 720-1,
	.ipipe_hst = 24,
	.ipipe_hsz = 1280-1,
	/*interupt generation after lines*/

	.def_cor = {	.dfc_en	= DISABLE,
			.dfc_sel = 0,
			//.dfc_adr = 0,
			.dfc_siz = 4,
			.dfc_table = NULL//(unsigned int*) yeeTable
		   },
	.prog_nf = {
			.noise_fil_en = DISABLE,
		        .d2f_cfg_spr = 0,
		        .d2f_cfg_shf = 0,
		        .type = 0,
		        .d2f_thr = NULL,//(unsigned int *)NoiseFilterTHR ,
		        .d2f_str = NULL//(unsigned int *)NoiseFilterSTR
		   },
	.prefilter = {
		     	.pre_en = DISABLE,
		        .sel_0 = 1,//AVG2MEDPIX,
		        .sel_1 = 1,
		        .typ_adaptive = ENABLE,
		        .typ_adaptive_dotred = DISABLE,
		        .pre_shf = 9,
		        .pre_gain = 128,
		        .pre_thr_g = 500,
		        .pre_thr_b = 4096,
		        .pre_thr_1 = 800
		     },
	.wb = 	   {
			.wb2_dgn  = 0x200,//512,//1023,//256,
		        .wb2_wg_r = 0xA0,
		        .wb2_wg_gr= 0x80,
		        .wb2_wg_gb= 0x80,
		        .wb2_wg_b = 0xA8//428
		   },
	.rgb2rgb = {
			 .rgb_mul_rr = 0x0224,
		         .rgb_mul_gr = 0x0EC0,
			 .rgb_mul_br = 0x001C,
		         .rgb_mul_rg = 0x0FAC,
		         .rgb_mul_gg = 0x01FB,
		         .rgb_mul_bg = 0x0F5A,
		         .rgb_mul_rb = 0x0FDF,
		         .rgb_mul_gb = 0x0F45,
		         .rgb_mul_bb = 0x01DC,
		         .rgb_oft_or = 0x0000,
			 .rgb_oft_og = 0x0000,
		         .rgb_oft_ob = 0x0000,
		         .gmm_cfg_bypr = GC_ENABLE,
		         .gmm_cfg_bypg = GC_ENABLE,
			 .gmm_cfg_bypb = GC_ENABLE,
		         .gmm_cfg_tbl  = IPIPE_ROM,
		         .gmm_cfg_siz = IPIPE_512,
		         .gmm_tbl_r = NULL,
		         .gmm_tbl_b = NULL,
		         .gmm_tbl_g = NULL,
		         .gmm_tbl_all = NULL//(unsigned int *)GammaTableall

		   },
	.rgb2yuv = {
			/* RDRV_IPIPE__SAT_LOW */
			.yuv_adj_ctr = 0x10,
		        .yuv_adj_brt = 0x00,

		        .yuv_mul_ry  = 0x004d,
		        .yuv_mul_gy  = 0x0096,
		        .yuv_mul_by  = 0x001d,
		        .yuv_mul_rcb = 0x03d4,
		        .yuv_mul_gcb = 0x03ac,
		        .yuv_mul_bcb = 0x0080,
		        .yuv_mul_rcr = 0x0080,
		        .yuv_mul_gcr = 0x0395,
		        .yuv_mul_bcr = 0x03eb,
		        .yuv_oft_y   = 0x00,
		        .yuv_oft_cb  = 0x80,
		        .yuv_oft_cr  = 0x80,
		        .yuv_y_min   = 0,
		        .yuv_y_max   = 0xFF,
		        .yuv_c_min   = 0,
		        .yuv_c_max   = 0xFF,
		        .yuv_phs_lpf = DISABLE,
		        .yuv_phs_position =  1,//0

		   },
 	.edge_enhancer = {

		 	        .yee_en =  DISABLE,
			        .yee_emf = DISABLE,
			        .yee_shf = 4,            /* HPF Down Shift
                                                    Value: valid range (0-15) */
				.yee_mul_00 = 48,
			        .yee_mul_01 = 12,
			        .yee_mul_02 = 1014,
			        .yee_mul_10 = 12,
			        .yee_mul_11 = 0,
			        .yee_mul_12 = 1018,
			        .yee_mul_20 = 1014,
			        .yee_mul_21 = 1018,
			        .yee_mul_22 = 1022,
			        .ee_table =NULL// (unsigned int*) yeeTable

			 },
	.false_color_suppresion = {
	  			     .fcs_en = ENABLE, // Uint8 csupEnable
				     .fcs_typ_typ = 0,
				     .fcs_shf_y = 0,
				     .fcs_shf_c = 7,
				     .fcs_thr = 235,
				     .fcs_sgn = 0,
				     .fcs_lth = 0
				  },
	.rsz_seq_seq = DISABLE,
	.rsz_seq_tmm = DISABLE,//output confined mode (normal mode)
	.rsz_seq_hrv = DISABLE,
	.rsz_seq_vrv = DISABLE,
	.rsz_seq_crv = DISABLE,

	.rsz_aal = ENABLE,

	.rsz_rsc_param ={
  			    {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 720-1,
			         .rsz_o_hsz = 1280-1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 256,
					//unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 256,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     },
			     {
			         .rsz_mode = CONTINUOUS,
			         .rsz_i_vst = 0,
			         .rsz_i_vsz = 0,
			         .rsz_i_hst = 0,
					 .rsz_o_vsz = 360-1,
			         .rsz_o_hsz = 640-1,
					 .rsz_o_hst = 0,
			         .rsz_v_phs = 0,
			         //unsigned int rsz_v_phs_o;
			         .rsz_v_dif = 512,
					//unsigned int rsz_v_siz_o;
			         .rsz_h_phs = 0,
			         .rsz_h_dif = 512,
			         .rsz_h_typ = CUBIC,
			         .rsz_h_lse_sel = INTERNAL_VALUE,
			         .rsz_h_lpf = 0
			     }
			},
	.rsz2rgb = {
			{
				.rsz_rgb_en = DISABLE,//....check
		/*		.rsz_rgb_typ = 0
				.rsz_rgb_msk0 = 0,
				.rsz_rgb_msk1 = 0,
				.rsz_rgb_alpha_val = 0 */
			   },
			{
				.rsz_rgb_en = DISABLE,//....check
			}
		   },

	.ext_mem_param = {
			    {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 1280 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 720
   			   },
		        {
			      .rsz_sdr_bad_h = 0,
			      .rsz_sdr_bad_l = 0,
			      .rsz_sdr_sad_h = 0,
			      .rsz_sdr_sad_l = 0,
			      .rsz_sdr_oft = 640 * 2,
			      .rsz_sdr_ptr_s = 0,
			     .rsz_sdr_ptr_e = 360
   			   },
			 },
	.rsz_en[0] = ENABLE,
	.rsz_en[1] = ENABLE
};

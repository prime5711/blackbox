set tabstop=4
:set tabstop=4
:map g %
:map t 0i	j
:map T 0xj
:map e 0i//  j
:map E 04xj
:set ts=4
":set nowrap
:set nu
:set ai
:set hlsearch
:syntax enable
syntax enable
":colorscheme    darkblue
"colorscheme    darkblue
":colorscheme    desert
"colorscheme    desert
:colorscheme    torte
colorscheme    torte

"set csprg=cswrapper.exe
":set csprg=cswrapper.exe

":ab #b  /********************************************************
":ab #e  ********************************************************/
:ab #f  printf "\n\n\t \033[22;30;31m                \033[0m \n\n"
:ab #g  printf "\n\n\t \033[22;30;31m                \033[0m \n\n"
:ab #h  printk("\n\t \033[22;30;31m line:%d:@%s in %s  \033[0m \n",__LINE__,__FUNCTION__,__FILE__  );
:ab #i  printk("\n\t \033[22;30;31m line:%d:@%s in %s  \033[0m \n",__LINE__,__FUNCTION__,__FILE__  );


" \033[22;30m - black
" \033[22;31m - red
" \033[22;32m - green
" \033[22;33m - brown
" \033[22;34m - blue
" \033[22;35m - magenta
" \033[22;36m - cyan
" \033[22;37m - gray
" \033[01;30m - dark gray
" \033[01;31m - light red
" \033[01;32m - light green
" \033[01;33m - yellow
" \033[01;34m - light blue
" \033[01;35m - light magenta
" \033[01;36m - light cyan
" \033[01;37m - white

"warning :red
:ab #w  INNO_SHCHO_PRINT("\n\n\t \033[01;32m line:%d:@%s in %s \n",__LINE__,__FUNCTION__,__FILE__  );
:ab #e  INNO_SHCHO_PRINT("\n\n\t \033[22;31m line:%d:@%s in %s \n",__LINE__,__FUNCTION__,__FILE__  );
:ab #m  INNO_SHCHO_PRINT("\n\n\t \033[22;35m line:%d:@%s in %s \n",__LINE__,__FUNCTION__,__FILE__  );




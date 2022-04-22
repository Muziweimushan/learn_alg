"
"  __  __           _              _ _             _
" |  \/  |_   _ ___(_)_      _____(_| )___  __   _(_)_ __ ___  _ __ ___
" | |\/| | | | |_  / \ \ /\ / / _ \ |// __| \ \ / / | '_ ` _ \| '__/ __|
" | |  | | |_| |/ /| |\ V  V /  __/ | \__ \  \ V /| | | | | | | | | (__
" |_|  |_|\__,_/___|_| \_/\_/ \___|_| |___/   \_/ |_|_| |_| |_|_|  \___|
"
syntax on 									" 自动语法高亮
set autoread  								" 文件在vim外修改过，自动重载
set nocompatible							" 去掉讨厌的有关vi一致性模式，避免以前版本的一些bug和局限
set confirm  								" 在处理未保存或只读文件时，弹出确认消息
filetype plugin indent on					" 侦测文件类型
set number 									" 显示行号

set clipboard=unnamedplus						" 共享剪贴板
"set textwidth=80							" 设置超过80行自动换行
set ruler 									" 打开状态栏标尺
set mouse=a 								" 设置在vim中可以使用鼠标
set magic 									" 设置魔术
set backspace=indent,eol,start"
"let mapleader = ' '
"let g:mapleader = ' '

"nmap <leader>w :w!<CR>

set incsearch 								" 输入搜索内容时就显示搜索结果
set hlsearch 								" 搜索时高亮显示被找到的文本
set showcmd									" 显示输入的命令
set showmatch								" 高亮括号匹配
set matchtime=1								" 匹配括号高亮的时间(十分之一秒)  
set matchpairs={:},(:),[:]						" 匹配括号"{}""()"  


" 检测文件编码时，优先考虑 UTF-8
set fileencodings=utf-8,ucs-bom,gbk,gb2312,big5,latin1
set langmenu=zh_CN.UTF-8


" 不同平台，设置不同的行尾符，即 EOL
" 注意：在 Mac 平台，也是 unix 优先；自 OS X 始，行尾符与 Unix 一致，
"      都是 `\n` 而不是 `\r`
if has("win32")
    set fileformats=dos,unix,mac
else
    set fileformats=unix,mac,dos
endif

" 在断行、合并(join)行时，针对多字节字符（比如中文）的优化处理
set formatoptions+=mM

set cindent	"设置C风格的缩进

set smarttab
"set expandtab | set tabstop=4 | set shiftwidth=4
set expandtab | set tabstop=4 | set shiftwidth=4
au FileType make set noexpandtab | set tabstop=4 | set shiftwidth=4

"高亮不想要的空格
highlight ExtraWhitespace ctermbg=red guibg=red
match ExtraWhitespace /\s\+$/
autocmd BufWinEnter * match ExtraWhitespace /\s\+$/
autocmd InsertEnter * match ExtraWhitespace /\s\+\%#\@<!$/
autocmd InsertLeave * match ExtraWhitespace /\s\+$/
"autocmd BufWinLeave * call clearmatches() " for performance


"命令模式下命令补全
set wildignore=log/**,node_modules/**,target/**,tmp/**,*.rbc
set wildmenu
set wildmode=longest,list,full


"copy to system clipboard when we at visual mode
"vnoremap Y :w !xclip -i -sel clip<CR>

"vim-plug插件开始
call plug#begin('~/.vim/plugged')

"在此处开始添加插件
Plug 'joshdick/onedark.vim'
Plug 'tomasr/molokai'
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
"Plug 'valloric/YouCompleteMe'
Plug 'scrooloose/nerdtree'
Plug 'majutsushi/tagbar'
Plug 'tomtom/tcomment_vim'
"Plug 'jeaye/color_coded'					"语法高亮插件
"这个需要lua,目前没有使能...
Plug 'justinmk/vim-syntax-extra'				"以下两个为语法高亮插件
Plug 'octol/vim-cpp-enhanced-highlight'
Plug 'ludovicchabant/vim-gutentags'				"代码跳转插件
Plug 'skywind3000/gutentags_plus'				"使得上面装的gutentags可以使用cscope/gtags来搜索函数调用
Plug 'skywind3000/vim-preview'

Plug 'Yggdroot/LeaderF'						"模糊搜索插件
"Plug 'dense-analysis/ale'					"代码分析插件
"Plug 'rdnetto/YCM-Generator'

"mardown 预览插件
Plug 'iamcco/mathjax-support-for-mkdp'
Plug 'iamcco/markdown-preview.vim'


Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'mbbill/undotree'
Plug 'morhetz/gruvbox'
Plug 'arzg/vim-colors-xcode'
Plug 'kien/rainbow_parentheses.vim'
Plug 'airblade/vim-rooter'
Plug 'junegunn/fzf'
Plug 'junegunn/fzf.vim'
Plug 'antoinemadec/coc-fzf'
"Plug 'jackguo380/vim-lsp-cxx-highlight'
"插件到此结束
call plug#end()


"set background=dark

colorscheme onedark
"colorscheme xcodelight
"colorscheme molokai
syntax enable

highlight Normal     ctermbg=NONE guibg=NONE
highlight LineNr     ctermbg=NONE guibg=NONE
highlight SignColumn ctermbg=NONE guibg=NONE

"let g:airline_theme='one'

"***********************************************
"                  NERDTree
"***********************************************
map <F2> :NERDTreeMirror<CR>
map <F2> :NERDTreeToggle<CR>
let NERDChristmasTree=1 				"	显示增强
let NERDTreeAutoCenter=1 				"	自动调整焦点
let NERDTreeMouseMode=2 				"	鼠标模式：目录单击，文件双击
let NERDTreeShowFiles=1 				"	显示文件
let NERDTreeShowHidden=1 				"	显示隐藏文件
let NERDTreeHightCursorline=1			"	高亮显示当前文件或目录
let NERDTreeShowLineNumbers=1			"	显示行号
let NERDTreeWinPos='left'				"	窗口位置
let NERDTreeWinSize=31 					"	窗口宽度
let NERDTreeMinimalUI=1 				"	不显示帮助
"
"************************************************
"                  tagbar
"************************************************
nmap <F8> :TagbarToggle<CR>


" gutentags搜索工程目录的标志，碰到这些文件/目录名就停止向上一级目录递归 "
let g:gutentags_project_root = ['.root', '.svn', '.git', '.project']

let g:gutentags_modules = ['ctags', 'gtags_cscope']

" 所生成的数据文件的名称 "
let g:gutentags_ctags_tagfile = '.tags'

" 将自动生成的 tags 文件全部放入 ~/.cache/tags 目录中，避免污染工程目录 "
let s:vim_tags = expand('~/.cache/tags')
let g:gutentags_cache_dir = s:vim_tags
" 检测 ~/.cache/tags 不存在就新建 "
if !isdirectory(s:vim_tags)
   silent! call mkdir(s:vim_tags, 'p')
endif

let g:gutentags_plus_switch = 1

" 配置 ctags 的参数 "
let g:gutentags_ctags_extra_args = ['--fields=+niazS', '--extras=+q']
let g:gutentags_ctags_extra_args += ['--c++-kinds=+px']
let g:gutentags_ctags_extra_args += ['--c-kinds=+px']

"使用universal tags要添加这一行
let g:gutentags_ctags_extra_args += ['--output-format=e-ctags', '--fields=+nS'] " 选项n为添加行号, S为保留函数签名

let g:gutentags_auto_add_gtags_cscope = 0
let g:gutentags_plus_nomap = 1
let g:gutentags_define_advanced_commands = 1
let g_gutentags_trace = 1

let $GTAGSLABLE = 'native-pygments'
let $GTAGSCONF = '/home/linuxbrew/.linuxbrew/Cellar/global/6.6.8/share/gtags/gtags.conf'

"s 查找符号出现的地方
noremap <silent> <leader>gs :GscopeFind s <C-R><C-W><cr>
noremap <silent> <leader>gg :GscopeFind g <C-R><C-W><cr>
noremap <silent> <leader>gc :GscopeFind c <C-R><C-W><cr>
noremap <silent> <leader>gt :GscopeFind t <C-R><C-W><cr>
noremap <silent> <leader>ge :GscopeFind e <C-R><C-W><cr>
noremap <silent> <leader>gf :GscopeFind f <C-R>=expand("<cfile>")<cr><cr>
noremap <silent> <leader>gi :GscopeFind i <C-R>=expand("<cfile>")<cr><cr>
"noremap <silent> <leader>gd :GscopeFind d <C-R><C-W><cr>
noremap <silent> <leader>ga :GscopeFind a <C-R><C-W><cr>

noremap <leader>b :cclose<CR>

"用于配置插件 vim-preview
noremap <F4> :PreviewSignature!<cr>
inoremap <F4> <c-\><c-o>:PreviewSignature!<cr>

autocmd FileType qf nnoremap <silent><buffer> p :PreviewQuickfix<cr>
autocmd FileType qf nnoremap <silent><buffer> P :PreviewClose<cr>

noremap <m-u> :PreviewScroll -1<cr>
noremap <m-d> :PreviewScroll +1<cr>
inoremap <m-u> <c-\><c-o>:PreviewScroll -1<cr>
inoremap <m-d> <c-\><c-o>:PreviewScroll +1<cr>

noremap <F3> :PreviewTag<cr>



"插件LeaderF的配置

let g:Lf_ShortcutF = '<c-p>'
let g:Lf_ShortcutB = '<m-n>'
noremap <c-n> :LeaderfMru<cr>
noremap <F7> :LeaderfFunction!<cr>
noremap <M-n> :LeaderfBuffer<cr>
noremap <M-m> :LeaderfTag<cr>
let g:Lf_StlSeparator = { 'left': '', 'right': '', 'font': '' }

let g:Lf_RootMarkers = ['.project', '.root', '.svn', '.git']
let g:Lf_WorkingDirectoryMode = 'Ac'
let g:Lf_WindowHeight = 0.30
let g:Lf_CacheDirectory = expand('~/.vim/cache')
let g:Lf_ShowRelativePath = 0
let g:Lf_HideHelp = 1
let g:Lf_StlColorscheme = 'powerline'
let g:Lf_PreviewResult = {'Function':0, 'BufTag':0}





"YCM配置
let g:ycm_global_ycm_extra_conf='~/.vim/plugged/YouCompleteMe/third_party/ycmd/.ycm_extra_conf.py'	"设置默认配置文件的路径
let g:ycm_min_num_identifier_candidate_chars = 2	"输入两个字符开始进行补全提示
let g:ycm_key_invoke_completion = '<c-z>'	"ctrl+z 触发补全
let g:ycm_server_log_level = 'info'
let g:ycm_collect_identifiers_from_comments_and_strings = 1	"注释也可以补全
let g:ycm_complete_in_strings = 1

"设置YCM不要弹出函数原型,通过另外插件实现此功能
set completeopt=menu,menuone
let g:ycm_add_preview_to_completeopt = 0

"YCM不要显示诊断信息
let g:ycm_show_diagnostics_ui = 0

noremap <c-z> <NOP>
"默认C文件只有输入->或.才进行补全,修改为只要输入2个字符以上就开始
let g:ycm_semantic_triggers = {
		\ 'c,cpp,python,java,go,erlang,perl' : ['re!\w{2}'],
		\ 'cs,lua,javascript' : ['re!w{2}'],
		\ }


"ALE配置
"let g:ale_completion_delay = 500
"let g:ale_echo_delay = 20
"let g:ale_lint_delay = 500
"let g:ale_echo_msg_format = '[%linter%] %code: %%s'
"let g:ale_lint_on_text_changed = 'normal'
"let g:ale_lint_on_insert_leave = 1
"let g:airline#extensions#ale#enabled = 1
"let g:ale_sign_column_always = 1
"let g:ale_sign_error = '>>'
"let g:ale_sign_waring = '--'
"
"let g:ale_c_gcc_options = '-Wall -O2 -std=c99'
"let g:ale_cpp_gcc_options = '-Wall -O2 -std=c++11'
"let g:ale_c_cppcheck_options = ''
"let g:ale_cpp_cppcheck_options = ''



set laststatus=2
set t_Co=256
set modifiable

set signcolumn=yes
set noshowmode

"markdown 预览
nmap <slient><c-m> <Plug>MarkdownPreview    "normal 模式下预览


" setting for coc.nvim
"
"nmap <slient> gd <Plug>(coc-definition)
nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)

nmap <silent> gD sl<Plug>(coc-definition)

" Use K to show documentation in preview window.
nnoremap <silent> K :call <SID>show_documentation()<CR>

function! s:show_documentation()
  if (index(['vim','help'], &filetype) >= 0)
    execute 'h '.expand('<cword>')
  elseif (coc#rpc#ready())
    call CocActionAsync('doHover')
  else
    execute '!' . &keywordprg . " " . expand('<cword>')
  endif
endfunction

" Use tab for trigger completion with characters ahead and navigate.
" NOTE: Use command ':verbose imap <tab>' to make sure tab is not mapped by
" other plugin before putting this into your config.
inoremap <silent><expr> <TAB>
      \ pumvisible() ? "\<C-n>" :
      \ <SID>check_back_space() ? "\<TAB>" :
      \ coc#refresh()
inoremap <expr><S-TAB> pumvisible() ? "\<C-p>" : "\<C-h>"

function! s:check_back_space() abort
  let col = col('.') - 1
  return !col || getline('.')[col - 1]  =~# '\s'
endfunction

" Use <c-space> to trigger completion.
if has('nvim')
  inoremap <silent><expr> <c-space> coc#refresh()
else
  inoremap <silent><expr> <c-@> coc#refresh()
endif

" Make <CR> auto-select the first completion item and notify coc.nvim to
" format on enter, <cr> could be remapped by other vim plugin
inoremap <silent><expr> <cr> pumvisible() ? coc#_select_confirm()
                              \: "\<C-g>u\<CR>\<c-r>=coc#on_enter()\<CR>"

" Use `[g` and `]g` to navigate diagnostics
" Use `:CocDiagnostics` to get all diagnostics of current buffer in location list.
nmap <silent> [g <Plug>(coc-diagnostic-prev)
nmap <silent> ]g <Plug>(coc-diagnostic-next)


" Highlight the symbol and its references when holding the cursor.
autocmd CursorHold * silent call CocActionAsync('highlight')

" Mappings for CoCList
" Show all diagnostics.
nnoremap <silent><nowait> <space>a  :<C-u>CocList diagnostics<cr>
" Manage extensions.
nnoremap <silent><nowait> <space>e  :<C-u>CocList extensions<cr>
" Show commands.
nnoremap <silent><nowait> <space>c  :<C-u>CocList commands<cr>
" Find symbol of current document.
nnoremap <silent><nowait> <space>o  :<C-u>CocList outline<cr>
" Search workspace symbols.
nnoremap <silent><nowait> <space>s  :<C-u>CocList -I symbols<cr>
" Do default action for next item.
nnoremap <silent><nowait> <space>j  :<C-u>CocNext<CR>
" Do default action for previous item.
nnoremap <silent><nowait> <space>k  :<C-u>CocPrev<CR>
" Resume latest coc list.
nnoremap <silent><nowait> <space>p  :<C-u>CocListResume<CR>


"修复奇葩字符输出问题
let &t_TI = ""
let &t_TE = ""
let g:coc_disable_startup_warning = 1

"let $GTAGSLABEL='native'
"set list

"配置ctags的tags文件路径以及文件名
set tags=./.tags;,.tags


"rainbow_parentheses
let g:rbpt_colorpairs = [
    \ ['brown',       'RoyalBlue3'],
    \ ['Darkblue',    'SeaGreen3'],
    \ ['darkgray',    'DarkOrchid3'],
    \ ['darkgreen',   'firebrick3'],
    \ ['darkcyan',    'RoyalBlue3'],
    \ ['darkred',     'SeaGreen3'],
    \ ['darkmagenta', 'DarkOrchid3'],
    \ ['brown',       'firebrick3'],
    \ ['gray',        'RoyalBlue3'],
    \ ['black',       'SeaGreen3'],
    \ ['darkmagenta', 'DarkOrchid3'],
    \ ['Darkblue',    'firebrick3'],
    \ ['darkgreen',   'RoyalBlue3'],
    \ ['darkcyan',    'SeaGreen3'],
    \ ['darkred',     'DarkOrchid3'],
    \ ['red',         'firebrick3'],
    \ ]
let g:rbpt_max = 16
let g:rbpt_loadcmd_toggle = 0

let g:rooter_patterns = ['.root', '.svn', '.git', '.project']

"set rainbow_parentheses alway on
au VimEnter * RainbowParenthesesToggle
au Syntax * RainbowParenthesesLoadRound
au Syntax * RainbowParenthesesLoadSquare
au Syntax * RainbowParenthesesLoadBraces

let g:fzf_layout = { 'window': { 'width': 0.9, 'height': 0.8 } }

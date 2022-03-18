set runtimepath^=~/.vim runtimepath+=~/.vim/after
let &packpath = &runtimepath
source ~/.vimrc


set relativenumber


"key mapping ...
noremap S :w<CR>

"use Y to copy till the end of the line
nnoremap Y y$

"copy data to system clipboard
vnoremap Y "+y

noremap <LEADER><CR> :nohlsearch<CR>


"Ctrl + U/D will move up/down without moving the cursor
noremap <C-U> 5<C-y>
noremap <C-D> 5<C-e>

" disable function for key s ...
noremap s <nop>

" split the screens to up (horizontal), down (horizontal), left (vertical), right (vertical)
noremap sk :set nosplitbelow<CR>:split<CR>:set splitbelow<CR>
noremap sj :set splitbelow<CR>:split<CR>
noremap sh :set nosplitright<CR>:vsplit<CR>:set splitright<CR>
noremap sl :set splitright<CR>:vsplit<CR>

" Resize splits with arrow keys
noremap <up> :res +5<CR>
noremap <down> :res -5<CR>
noremap <left> :vertical resize-5<CR>
noremap <right> :vertical resize+5<CR>

" Place the two screens up and down
noremap sh <C-w>t<C-w>K
" Place the two screens side by side
noremap sv <C-w>t<C-w>H

" Rotate screens
noremap srh <C-w>b<C-w>K
noremap srv <C-w>b<C-w>H

" Press <SPACE> + q to close the window below the current window
"noremap <LEADER>q <C-w>j:q<CR>

" Spelling Check with <space>sc
noremap <LEADER>sc :set spell!<CR>



" ===
" === Undotree
" ===
noremap L :UndotreeToggle<CR>
let g:undotree_DiffAutoOpen = 1
let g:undotree_SetFocusWhenToggle = 1
let g:undotree_ShortIndicators = 1
let g:undotree_WindowLayout = 2
let g:undotree_DiffpanelHeight = 8
let g:undotree_SplitWidth = 24
"function g:Undotree_CustomMap()
"	nmap <buffer> u <plug>UndotreeNextState
"	nmap <buffer> e <plug>UndotreePreviousState
"	nmap <buffer> U 5<plug>UndotreeNextState
"	nmap <buffer> E 5<plug>UndotreePreviousState
"endfunc
"
"

set smarttab
"set expandtab | set tabstop=4 | set shiftwidth=4
set expandtab | set tabstop=4 | set shiftwidth=4
au FileType make set noexpandtab | set tabstop=4 | set shiftwidth=4


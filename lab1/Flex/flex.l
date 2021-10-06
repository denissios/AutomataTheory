%option noyywrap
%x SERVER TAIL
%%
<INITIAL>{
^([gG][tT][aA][lL][kK]:([cC][hH][aA][tT]|[tT][aA][lL][kK])\?[jJ][iI][dD]=[a-zA-Z0-9]+@) BEGIN(SERVER);
\n return 0;
. ;
}
<SERVER>{
([a-zA-Z0-9]{1,})/(\.[a-zA-Z]{1,4})\n {BEGIN(TAIL); return 1;}
. BEGIN(INITIAL); 
\n {BEGIN(INITIAL); return 0;}
}
<TAIL>
{
. ;
\n BEGIN(INITIAL);
}
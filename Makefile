#================================[[ beg-code ]]================================#



#===(manditory)============================================#
# basename of executable, header, and all code files
NAME_BASE  = charybdis
# additional standard and outside libraries
LIB_STD    = 
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lX11 -lXext -lGL -lySTR_debug -lyX11_debug -lyEXEC_debug -lyPARSE_debug -lyFONT_debug -lm
#  -lGL -lGLU
# directory for production code, no trailing slash
INST_DIR   = /usr/local/bin



#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = 
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/zeno_make



#===(post-processing)======================================#
# create a rule for...
install_post       :

#remove_post        :



#================================[[ end-code ]]================================#

ADP_LIB_DIR=$HOME"/Documents/MyCode/TestMDPLib/ADPFrameworkLib"

g++ -I $ADP_LIB_DIR/include -L $ADP_LIB_DIR/lib -static src/* main.cpp -I./include -lADPFramework -o Example2
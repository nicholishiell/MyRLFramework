ADP_LIB_DIR=$HOME"/Documents/MyCode/MyMDPCode/MyRLFramework/ADPFrameworkLib"

#g++ -I $ADP_LIB_DIR/include -L $ADP_LIB_DIR/lib -static src/* main.cpp -I./include -lADPFramework -g -o Example6
g++ -I $ADP_LIB_DIR/include -L $ADP_LIB_DIR/lib -static src/* main.cpp -I./include -lADPFramework -O3 -o Example6

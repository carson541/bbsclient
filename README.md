a bbs client terminal
=====================

x) generate makefile
* tmake qtopia.pro > Makefile

x) compile
* make

x) install on simulator
* cp bbsclient.desktop $QPEDIR/apps/Applications/
* cp bbsclient.png $QPEDIR/pics/
* cp bbsclient $QPEDIR/bin/
* cp bbsclient.html $QPEDIR/help/html/

x) run simulator
* qvfb &
* qpe -qws

x) install on sharp
* cp bbsclient.desktop $QPEDIR/sharp/apps/Applications/
* cp bbsclient.png $QPEDIR/sharp/pics/
* cp bbsclient $QPEDIR/sharp/bin/
* cp bbsclient.html $QPEDIR/sharp/help/html/

x) build package
* mkipks bbsclient.control

To compile the sources in dev/, please use:
- Qt 4.7.4
- Qt Creator 2.4.0 or above
- install Ipseity kernel source codes prior to compiling any environment
- install Ipseity kernel prior to using any environment
- If you intend to use the Prolog module of the VersatileCognitiveSystem, install SWI-Prolog 5.6.49 (available at software/) in dev/API/


Before executing the applications in IpseityProject/, please:
- Update the system environment variable Path with the absolute paths towards the directories lib\win\Qt_Libraries.
  Add the path at the beginning of the environment variable Path.
- Make sure the last driver of your graphics card has been installed. OpenGL 3.x must be supported.
- If you use the Prolog module of the VersatileCognitiveSystem, install SWI-Prolog 5.6.49 (available at software/) and make sure the path towards the binaries of SWI-Prolog is added into the environment variable Path.
- Install wgnuplot (for automatic generation of performance figures when using RLCognitiveSystems).

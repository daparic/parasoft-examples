This SpeedSensor example is a downloadable kernel module project that is meant to be used in 
Wind River Workbench with VxWorks 6.9. The unit tests that are provided with this example can be 
executed by running the following sequence of C/C++test's test configurations:

1. Builtin > Embedded Systems > Wind River > Extract Symbols from VxWorks Image
2. Builtin > Embedded Systems > Wind River > Workbench 3.x > Build VxWorks Test Module - DKM (PassFS)
3. Builtin > Embedded Systems > Wind River > Workbench 3.x > Load and Run VxWorks Test Object (DKM)
4. Builtin > Utilities > Load Test Results (Files)

Some test configurations, especially 1 and 2, may require additional customization. 
Please refer to the user guide for additional information about testing in Wind River Workbench.

# CMake generated Testfile for 
# Source directory: /home/ruicampos/Desktop/IC/IC_Project2/opencv/modules/objdetect
# Build directory: /home/ruicampos/Desktop/IC/IC_Project2/build/modules/objdetect
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_objdetect "/home/ruicampos/Desktop/IC/IC_Project2/build/bin/opencv_test_objdetect" "--gtest_output=xml:opencv_test_objdetect.xml")
set_tests_properties(opencv_test_objdetect PROPERTIES  LABELS "Main;opencv_objdetect;Accuracy" WORKING_DIRECTORY "/home/ruicampos/Desktop/IC/IC_Project2/build/test-reports/accuracy" _BACKTRACE_TRIPLES "/home/ruicampos/Desktop/IC/IC_Project2/opencv/cmake/OpenCVUtils.cmake;1769;add_test;/home/ruicampos/Desktop/IC/IC_Project2/opencv/cmake/OpenCVModule.cmake;1375;ocv_add_test_from_target;/home/ruicampos/Desktop/IC/IC_Project2/opencv/cmake/OpenCVModule.cmake;1133;ocv_add_accuracy_tests;/home/ruicampos/Desktop/IC/IC_Project2/opencv/modules/objdetect/CMakeLists.txt;2;ocv_define_module;/home/ruicampos/Desktop/IC/IC_Project2/opencv/modules/objdetect/CMakeLists.txt;0;")
add_test(opencv_perf_objdetect "/home/ruicampos/Desktop/IC/IC_Project2/build/bin/opencv_perf_objdetect" "--gtest_output=xml:opencv_perf_objdetect.xml")
set_tests_properties(opencv_perf_objdetect PROPERTIES  LABELS "Main;opencv_objdetect;Performance" WORKING_DIRECTORY "/home/ruicampos/Desktop/IC/IC_Project2/build/test-reports/performance" _BACKTRACE_TRIPLES "/home/ruicampos/Desktop/IC/IC_Project2/opencv/cmake/OpenCVUtils.cmake;1769;add_test;/home/ruicampos/Desktop/IC/IC_Project2/opencv/cmake/OpenCVModule.cmake;1274;ocv_add_test_from_target;/home/ruicampos/Desktop/IC/IC_Project2/opencv/cmake/OpenCVModule.cmake;1134;ocv_add_perf_tests;/home/ruicampos/Desktop/IC/IC_Project2/opencv/modules/objdetect/CMakeLists.txt;2;ocv_define_module;/home/ruicampos/Desktop/IC/IC_Project2/opencv/modules/objdetect/CMakeLists.txt;0;")
add_test(opencv_sanity_objdetect "/home/ruicampos/Desktop/IC/IC_Project2/build/bin/opencv_perf_objdetect" "--gtest_output=xml:opencv_perf_objdetect.xml" "--perf_min_samples=1" "--perf_force_samples=1" "--perf_verify_sanity")
set_tests_properties(opencv_sanity_objdetect PROPERTIES  LABELS "Main;opencv_objdetect;Sanity" WORKING_DIRECTORY "/home/ruicampos/Desktop/IC/IC_Project2/build/test-reports/sanity" _BACKTRACE_TRIPLES "/home/ruicampos/Desktop/IC/IC_Project2/opencv/cmake/OpenCVUtils.cmake;1769;add_test;/home/ruicampos/Desktop/IC/IC_Project2/opencv/cmake/OpenCVModule.cmake;1275;ocv_add_test_from_target;/home/ruicampos/Desktop/IC/IC_Project2/opencv/cmake/OpenCVModule.cmake;1134;ocv_add_perf_tests;/home/ruicampos/Desktop/IC/IC_Project2/opencv/modules/objdetect/CMakeLists.txt;2;ocv_define_module;/home/ruicampos/Desktop/IC/IC_Project2/opencv/modules/objdetect/CMakeLists.txt;0;")
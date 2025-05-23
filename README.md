# DiagnosticTools
 <div align="justify">
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Select the source code version to download Qt6 or Qt5. The plugin has been tested on Linux. My project has not been tested on MacOS and Windows, but the code contains everything needed to build for these operating systems.<br>
 </div>
 
 ![qtcreator9](https://github.com/user-attachments/assets/77e874e0-4e23-4d66-9632-fd5fbf99b04c)
 ![qtcreator16](https://github.com/user-attachments/assets/f5fb3519-10c7-4c5d-b1cc-88ca71f5b8d9)

 <div align="justify">
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;To create a Qt Creator IDE plugin, you must first download the required library "Qt Creator (version number) Plugin Development". This library is not installed by default in the Linux repository, and if you installed the Qt Creator IDE from the Linux repository, then when you start building the starter plugin and any other, you will get an error. To solve this problem, it is best to download the Qt Creator IDE separately with the "Qt Creator Plugin Development" library already installed. This will also allow you to follow the recommendations from the Qt Creator website. To do this, you can download the offline installer for your version https://download.qt.io/, or download the Qt Creator IDE from the online installer, including the required library of tools "Qt Creator Plugin Development" on the side.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; You must make sure that you are using the same version of the QT build that you used to create the plugin. Due to binary compatibility rules and compatibility with the QT build source, the QT Creator plugin wizard creates a plugin that can only work with the same version of the QT build with which it was created.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Make sure that your Qt builder has all the necessary tools (modules) for building such as sudo apt install qml6-module-* and others. It is best to use the Qt Creator builder, which was built from source code and added to the Qt Creator IDE.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Very important. Correct the CmakeLists.txt file, you need to specify the installation directory of your Qt Creator IDE with the "Qt Creator (version number) Plugin Development" tool library.

<blockquote>
# Remove when sharing with others.<br>
list(APPEND CMAKE_PREFIX_PATH "the path to your assembly/qtcreator-X.X.X")<br>
# Find the ProjectExplorer library.<br>
find_library(PROJECT_EXPLORER_LIB NAMES ProjectExplorer PATHS "the path to your assembly"~/qtcreator-X.X.X/lib/qtcreator/plugins)<br>
</blockquote>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Please note that https://download.qt.io/archive/qtcreator/6.0/6.0.2/ for Qt 5 and for Qt 6 https://download.qt.io/official_releases/qtcreator/13.0/13.0.0/ are the versions that are included in the Ubuntu 22.04 and Ubuntu 24.04 linux repositories. I compiled my version of the plugin for these 2 versions. The build for Qt 5 for Linux Ubuntu 22.04 has some features as well as for Qt 6. In general, the standard template for Qt 5 is missing and instead of it there are libraries for Qt 6. This is due to the fact that the IDE Qt Creator 6.0.2 is built using Qt 6.2.2, to solve this problem it is enough to simply rewrite the library that is installed from the offline installer. For this, I made a small patch with all the changes. You can download the patch with the changes from the link: https://drive.google.com/file/d/14fK6uCNsLBagQxhPdJmHqxqpJ67LsyoP/view?usp=sharing. In this patch you can see all the changes exactly, there are not many of them and they are easy to understand, they are related to the location of Qt 5 libraries. For other versions of the Qt Creator IDE, you can simply change one line in the file path to code/lib/cmake/QtCreator/FindQt5.cmake <br>
-find_package(Qt6 ${Qt5_FIND_VERSION} CONFIG COMPONENTS Core QUIET)<br>
+find_package(Qt5 ${Qt5_FIND_VERSION} CONFIG COMPONENTS Core QUIET)<br>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;It is also worth noting that links to library files change from version to version, and they should be checked manually when compiling the code in case of an error. This concerns the difference between the versions of my Qt5 and Qt6 plugin, this may also concern the difference between different versions of Qt5 and Qt6. There is also a difference in the Diagnostictools.json.in file, the structure of this plugin signature file itself changes from version to version. To build a basic Qt Creator plugin specifically for your version of the Qt Creator IDE, since this template by default will contain exactly the structure that is relevant for your version, you need to read the instructions on the Qt Creator website https://doc.qt.io/qtcreator-extending/first-plugin.html Creating Your First Plugin.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;To add the compiled plugin to the Qt Creator IDE, you need the release version of the plugin /build/Qt_6_4_2_Qt_6_4_2-Release/lib/qtcreator/plugins/libDiagnostictools.so add to the directory with other plugins, the directory /usr/lib/x86_64-linux-gnu/qtcreator/plugins/ or yours at a similar address, where the Qt Creator IDE is installed.<br>
command example:<br>
sudo cp '/home/vladimirpve/Doc/plagin/Diagnostictools Qt_6_4_2/build/Qt_6_4_2_Qt_6_4_2-Release/lib/qtcreator/plugins/libDiagnostictools.so' /usr/lib/x86_64-linux-gnu/qtcreator/plugins/<br>

</div>

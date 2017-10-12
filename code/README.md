Rift Warrior
===========

Okay this file itself is the GitHub Markdown file for project Rift Warrior, and it will be used to generate GitHub's html summary of this project.

Build Errors: 0 ! Warnings: 0! 
-----------

Now the problem is it cannot be launched in WP8 emulator!
-----------
For now a FileNotFoundException happens at
...
        private void DrawingSurfaceBackground_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_d3dInterop == null)
            {
--->            m_d3dInterop = new Direct3DInterop();
...
...

PROBLEM SOLVED!

Like put a random sqlite3.dll and sqlite3.lib somewhere under the project directory tree and I just picked them to use. But in fact even the dll seems okay by some tools but it is for Windows instead of Windows Phone. It has some dependencies to system dlls that have been disabled on Windows Phone.
I finally get rid of that sqlite3.dll/lib and added the SQLite3 for Windows Phone via the Visual Studio's external functionality and add it as reference to project, pack matching build configuration and platform arch dll to package, and boom the app runs with no problem now!

Lesson learned here is, always check and ask the lib if its source is unknown or in doubt!

0. How to start this project in your Windows work station for Windows Phone app?
-----------
- First of all, you need to downloand and install free dev environment tools: 
    Windows Phone 8 SDK                                 
    - http://download.microsoft.com/download/9/3/8/938A5074-461F-4E3D-89F4-5CE2F42C1E36/WPexpress_full.exe
    Visual Studio 2013 express for Windows (not for Windows Desktop)    
    - http://www.visualstudio.com/downloads/download-visual-studio-vs#d-express-windows-8
    Windows Phone 8.1 SDK (this project was created as WP8 project instead of WP8.1 but we have to steal some header and libs from 8.1 SDK). Note this is NOT Windows SDK for Windows 8.1)
    - http://www.microsoft.com/en-us/download/details.aspx?id=43719
- Manually copy header files and libs below from 8.1 SDK directory to 8.0 SDK's (%ProgramFiles(x86)%\Windows Phone Kits\8.0\Include\ -> ..\8.1\Include\ and Lib\x86|arm)
    - XmlLite.h and XmlLite.lib

    And this is a page that lists all needed resources, http://dev.windows.com/en-us/develop/download-phone-sdk. It is a little confusing still though.  
- In a command window, run the SetEnv.bat.
- Launch the "Microsoft Visual Studio Express 2013 for Windows", which should be somethign like ...\IDE\VSWinExpress.exe, to open the Projects\RiftWarrior\proj.wp8-xaml\RiftWarrior.sln.
- NOTE, the "Microsoft Visual Studio Express 2012 for Windows Phone" keeps crashing while opening this project, so let us avoid wasting time using that old tool.
1. Whatever else...
-----------
Yep, as you can tell this is an unfinished summary page. More stuff will be coming...

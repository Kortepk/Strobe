cd /d "A:\0\AmbiboxEffects" &msbuild "AmbiboxEffects.vcxproj" /t:sdvViewer /p:configuration="Debug" /p:platform="Win32" /p:SolutionDir="A:\0" 
exit %errorlevel% 
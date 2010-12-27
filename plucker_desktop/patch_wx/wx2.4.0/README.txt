
Updates for Executing a commandline on Mac OSX, and for building 
on a POSIX-compliant system.

wx-config.in goes in and overwrites <wxdir>/wx-config.in
execute.h goes in and overwrites <wxdir>/include/wx/unix/execute.h
utilsunx.cpp goes in and overwrites <wxdir>/src/unix/utilsunx.cpp
utilsexc.cpp goes in and overwrites <wxdir>/src/mac/utilsexc.cpp

Note: to use the new wx-config.in, you obviously need to run configure to
generate a new wx-config from the wx-config.in file.

call gyp build.gyp -DOS=win --depth=. -f msvs -G msvs_version=2017
call MSBuild build.sln /t:Build /p:Configuration=Release /p:Platform=x64
"C:\Program Files\Microsoft SDKs\Windows\v7.0A\Bin\sn.exe" -k key.snk
"C:\Program Files\Microsoft SDKs\Windows\v7.0A\Bin\tlbimp" WebKit.tlb /keyfile:key.snk /out:WebKit.Interop.dll
"C:\Program Files\Microsoft SDKs\Windows\v7.0A\Bin\sn.exe" -R WebKit.Interop.dll key.snk
"C:\Program Files\Microsoft SDKs\Windows\v7.0A\Bin\mt.exe" -tlb:WebKit.tlb -dll:WebKit.dll -out:OpenWebKitSharp.manifest
pause
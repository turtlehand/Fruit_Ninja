del /Q "..\..\External\Include\Engine\"
xcopy /s /y /exclude:exclude_list.txt ".\*.h" "..\..\External\Include\Engine\"
xcopy /s /y /exclude:exclude_list.txt ".\*.inl" "..\..\External\Include\Engine\"
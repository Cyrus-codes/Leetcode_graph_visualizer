$b = "bin"
if (-not (Test-Path $b)) {
    New-Item -ItemType Directory -Path $b | Out-Null
}
$c = "g++"
$f = @("-std=c++17", "-O3", "-Wall", "-Wextra")
& $c $f "graphcpp/a.cpp" -o "$b/a.exe"

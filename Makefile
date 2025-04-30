generate:
	cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DBUILD_EXAMPLE=On -S . -B build

compile:
	cmake --build build

execute:
	cmd /c start .\build\bin\console.exe
	cmd /c start .\build\bin\emitter.exe

run: generate compile execute 

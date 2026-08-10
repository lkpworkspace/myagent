# myagent
learn ai agent

# 构建
```sh
mypm install -p ${PWD}/output --one-folder forgeant,0.2.0

cmake -S . -B build_proj -DCMAKE_INSTALL_PREFIX=output -DCMAKE_PREFIX_PATH=output -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
cmake --build build_proj -j --config Release --target install
```

# myagent
从网络中读取输入，调用大模型输出结果

# 构建
```sh
mypm install -p ${PWD}/output --one-folder forgeant,0.2.0
mypm install -p ${PWD}/output --one-folder myframe,1.0.2

cmake -S . -B build_proj -DCMAKE_INSTALL_PREFIX=output -DCMAKE_PREFIX_PATH=output -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
cmake --build build_proj -j --config Release --target install
```

# 执行
```sh
# for macos
export DYLD_LIBRARY_PATH=${PWD}/output/lib:${DYLD_LIBRARY_PATH}
# for unix like
export LD_LIBRARY_PATH=${PWD}/output/lib:${LD_LIBRARY_PATH}

./output/bin/launcher
```

# TODOLIST
- 工具
    - [ ] 微信聊天工具
    - [ ] 网页搜索工具
    - [ ] 本地终端命令执行工具
    - [ ] 定时执行工具
- [ ] 技能库
- [ ] 本地知识库

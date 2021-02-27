cmake -S. -Bbuild -G Xcode -T buildsystem=1 -DCMAKE_SYSTEM_NAME=iOS
cmake --build ./build --config Release --target install

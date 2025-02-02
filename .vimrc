command! -nargs=1 Init  :!cmake -S . -B <f-args> -DCMAKE_BUILD_TYPE=<f-args> -DCMAKE_INSTALL_PREFIX=<f-args> -DBUILD_TESTS=TRUE
command! -nargs=1 Build :!cmake --build <f-args> --parallel && cmake --install <f-args>
command! -nargs=1 InitBuild  :!cmake -S . -B <f-args> -DCMAKE_BUILD_TYPE=<f-args> -DCMAKE_INSTALL_PREFIX=<f-args> -DBUILD_TESTS=TRUE && cmake --build <f-args> --parallel && cmake --install <f-args> && ln -sf <f-args>/compile_commands.json .
command! -nargs=1 Test  :!ctest --test-dir <f-args>/tests
command! -nargs=1 TestV :!ctest --test-dir <f-args>/tests --verbose
command! -nargs=1 Run :!./<f-args>/bin/WindowManager
command! -nargs=1 CBuild :!cmake --build <f-args> --target clean && cmake --build <f-args> --parallel && cmake --install <f-args>
command! -nargs=1 TBuild :!cmake --build DEBUG --target <f-args> --parallel
command! -nargs=1 TTest :!ctest --test-dir DEBUG/tests -R <f-args>
command! -nargs=1 TTestV :!ctest --test-dir DEBUG/tests -R <f-args> --verbose

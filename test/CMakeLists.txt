# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(srtn-calcTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(srtn-calc_test source/srtn-calc_test.cpp)
target_link_libraries(
    srtn-calc_test PRIVATE
    srtn-calc_lib
    Catch2::Catch2WithMain
)
target_link_libraries(srtn-calc_test PRIVATE OpenGL::GL)
target_link_libraries(srtn-calc_test PRIVATE srtn-calc_lib)
target_link_libraries(srtn-calc_test PRIVATE imgui::imgui)
target_link_libraries(srtn-calc_test
	PRIVATE
	$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

target_compile_features(srtn-calc_test PRIVATE cxx_std_20)

catch_discover_tests(srtn-calc_test)

# ---- End-of-file commands ----

add_folders(Test)

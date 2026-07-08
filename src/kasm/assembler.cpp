int main(int argc, char** argv) {
    const char* assemblyCode = R"(
        li r0, 0x6942
        shl r0, 0x10
        li r0, 0xf00d
        jip +0x00000
    )";
    return 0;
}

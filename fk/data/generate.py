#!/usr/bin/python3

for name in [ "animals", "adjectives" ]:
    with open(name + ".txt", 'r') as f:
        all = f.readlines()
        with open(name + ".h", 'w') as h_file:
            h_file.write("constexpr size_t %s_size = (%d);\n" % (name, len(all)))
            h_file.write("const char *%s[%d] = {\n" % (name, len(all) + 1))
            for c, line in enumerate(all):
                h_file.write("  \"%s\",\n" % (line.strip().capitalize()))
            h_file.write("  NULL\n")
            h_file.write("};\n")

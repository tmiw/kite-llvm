t = make System.collections.binary_tree();
t|set("apple", "mac");
t|set("ibm", "pc");
t|set("sun", "unix");

t|reset;
while (t|next)
[
    i = t|cur;
    i[0]|print;
    i[1]|print;
];
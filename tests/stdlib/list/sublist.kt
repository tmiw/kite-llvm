v = make System.list();
v = v|append("X");
v = v|append("Y");
v = v|append("Z");
v|sublist(1)|print;
v|sublist(0, 1)|print;
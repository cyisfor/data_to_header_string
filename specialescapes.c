case '\\':
	PUT("\\",1);
	++count;
	break;

case '\"':
	PUT("\"",1);
	++count;
	break;

case '\0':
	PUT("0",1);
	++count;
	break;
case '\a':
	PUT("a",1);
	++count;
	break;
case '\b':
	PUT("b",1);
	++count;
	break;
case '\f':
	PUT("f",1);
	++count;
	break;
case '\n':
	PUT("n",1);
	++count;
	break;
case '\r':
	PUT("r",1);
	++count;
	break;
case '\v':
	PUT("v",1);
	++count;
	break;

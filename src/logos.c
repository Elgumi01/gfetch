typedef struct {
    const char *name;
    const char *ascii;
} Logo;

const Logo logos[] = {
	{
		"arch",
        	"   /\\\n"
        	"  /  \\\n"
        	" /____\\\n"
	}
};

const int logo_count = sizeof(logos) / sizeof(logos[0]);

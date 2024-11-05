DIR_BUILD = build
DIR_INTER = interpolator

fix_format_cpp:
	find $(DIR_INTER)/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i --verbose	

fix_format_py:
	black $(DIR_INTER)/

fix_format: fix_format_cpp fix_format_py

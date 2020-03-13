echo "GOING TO ERASE THE for-cbmc FOLDER AND REGENERATE IT"
read -p "Are you sure? [Y/N] " -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
  rm -rf for-cbmc
  cp -R for-gillian for-cbmc

  for filename in "for-cbmc"/**/*.c; do
    echo "transforming $filename"
    sed -i '' -e '1s/^/int nondet_int(); char nondet_char(); /' $filename
    comby 'int :[[x]] = __builtin_annot_intval("symb_int", :[[x]]);' 'int :[x] = nondet_int();' -d $(dirname $filename) $(basename $filename) -i
    comby 'char :[[x]] = __builtin_annot_intval("symb_int", :[[x]]);' 'char :[x] = nondet_char();' -d $(dirname $filename) $(basename $filename) -i
    comby 'char :[[x]] = (char) __builtin_annot_intval("symb_int", :[[x]]);' 'char :[x] = nondet_char();' -d $(dirname $filename) $(basename $filename) -i
    comby ':[[x]] = __builtin_annot_intval("symb_int", :[[x]]);' ':[x] = nondet_int();' -d $(dirname $filename) $(basename $filename) -i # FIXME: timeout
    comby '*:[[x]] = __builtin_annot_intval("symb_int", *:[[x]]);' '*:[x] = nondet_int();' -d $(dirname $filename) $(basename $filename) -i
    comby 'ASSERT(:[assertion]);' 'assert(:[assertion]);' -d $(dirname $filename) $(basename $filename) -i
    comby 'ASSUME (:[assertion]);' '__CPROVER_assume(:[assertion]);' -d $(dirname $filename) $(basename $filename) -i
  done

  for filename in "for-cbmc"/**/common.h; do
    echo "transforming $filename"
    sed -i '' -e 's/#include <gillian-c\/gillian-c.h>//g' $filename
  done
fi

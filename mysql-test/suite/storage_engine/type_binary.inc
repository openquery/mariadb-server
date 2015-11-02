#
# BINARY column types
#

--disable_warnings
DROP TABLE IF EXISTS t1;
--enable_warnings

let $create_definition =
  b BINARY $col_opts,
  b0 BINARY(0) $col_opts,
  b1 BINARY(1) $col_opts,
  b20 BINARY(20) $col_opts,
  b255 BINARY(255) $col_opts,
  b256 BINARY(256) $col_opts,
  b8000 BINARY(8000) $col_opts
;
--source create_table.inc
if ($mysql_errname)
{
  --let $my_last_stmt = $create_statement
  --let $functionality = BINARY types
  --source unexpected_result.inc
}
if (!$mysql_errname)
{
  --replace_column 3 # 4 # 5 # 6 #
  SHOW COLUMNS IN t1;

  # Valid values 

  INSERT INTO t1 (b,b0,b1,b20,b255,b256,b8000) VALUES ('','','','','','','');
  INSERT INTO t1 (b,b0,b1,b20,b255,b256,b8000) VALUES ('a','','b','abcdefghi klmnopqrst', 'Creating an article for the Knowledgebase is similar to asking questions. First, navigate to the category where you feel the article should be. Once there, double check that an article doesn\'t already exist which would work.', REPEAT('puppydog', 8), REPEAT('lots of cats are gay', 400));

  --sorted_result
  SELECT HEX(b), HEX(b0), HEX(b1), HEX(b20), HEX(b255), HEX(b256), HEX(b8000) FROM t1;

  # Invalid values

  INSERT INTO t1 (b,b0,b1,b20,b255,b256,b8000) VALUES ('abc', 'a', 'abc', REPEAT('a',21), REPEAT('x',256), REPEAT('y',257), REPEAT('z',8001));
  INSERT INTO t1 (b,b0,b1,b20,b255,b256,b8000) SELECT b255, b255, b255, b255, b8000, b8000, CONCAT(b8000, b8000) FROM t1;

  --sorted_result
  SELECT HEX(b), HEX(b0), HEX(b1), HEX(b20), HEX(b255), HEX(b256), HEX(b8000) FROM t1;

  --let $error_codes = ER_TOO_BIG_FIELDLENGTH
  --let $alter_definition = ADD COLUMN b20000 BINARY(20000) $col_opts
  --source alter_table.inc
  --replace_column 3 # 4 # 5 # 6 #
  SHOW COLUMNS IN t1;

  DROP TABLE t1;
}


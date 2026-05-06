# sample_error.dsl — Should trigger a semantic error
# 'z' is used but never assigned

begin

x = 5;

print z + x;

end

function some_function()
{
	var i = 0;
	while(true)
	{
		i++;
		if (i == 8)
		{
			break;
		}
		My.Echo("some_function");
	}	
}

function empty_function()
{
	My.Echo("empty_function");
}

function some_other_function()
{
	My.Spawn("empty_function");
	var i = 0;
	while(true)
	{
		i++;
		if (i == 14)
		{
			i = 0;
			My.Spawn("some_function");
		}
		My.Echo("some_other_function");
	}	
}

My.Spawn("some_other_function"); // What I really want is to pass some_other_function itself, not a string

while(true)
{
	My.Echo("main");
}

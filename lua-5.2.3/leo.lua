
function fact(n)
	if n == 0 then
		return 1
	else
		return n * fact(n-1)
	end
end

function norm(x,y)
	local r = x*x + y*y
	return sqrt(r)
end

function sqrt(n)
	for i=1,1000 do
		if i*i==n then return i end
	end
	return -1
end

print("enter a number:")
a = io.read("*number")        -- read a number
print(fact(a))
print(norm(3,4))

-- print(loadlib())

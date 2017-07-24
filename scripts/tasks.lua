function wait( t )
    local start = os.time()
    while( os.time() < start + t ) do
        coroutine.yield()
    end
end

function tick( co, t )
    ret, error = coroutine.resume( co, t )
    if not ret and error ~= 'cannot resume dead coroutine' then 
        print( error )
    end
end


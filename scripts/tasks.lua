function wait( t )
    local start = os.time()
    while( os.time() < start + t ) do
        coroutine.yield()
    end
end

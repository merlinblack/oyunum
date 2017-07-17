function dozipfile( file )
    local zzlib = require'zzlib/zzlib'

    out,err = zzlib.gunzipf( file )

    if not out then
        return nil, err
    end
    
    code, err = loadstring( out, file )

    if code then 
        return code()
    else
        return nil, err
    end
end

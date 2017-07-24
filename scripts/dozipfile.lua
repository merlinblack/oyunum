function dozipfile( file )
    local zzlib = require'zzlib/zzlib'
    local out, err, code

    out, err = zzlib.gunzipf( file )

    if not out then
        return nil, err
    end
    
    code, err = load( out, file )

    if code then 
        return code()
    else
        return nil, err
    end
end

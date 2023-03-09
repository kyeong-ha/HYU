fun merge(xs: int list, ys: int list): int list =
    if null xs andalso null ys then []
    else if null xs then ys
    else if null ys then xs
    else
        if (hd xs = hd ys) then []
        
        else if (hd xs < hd ys) then (hd xs) :: merge(tl xs, ys)
        else (hd ys) :: merge(xs, tl ys);


fun reverse(xs: int list): int list = 
    let
        fun rev_aux(ys: int list, res: int list): int list =
            if null ys then res
            else rev_aux(tl ys, (hd ys) :: res)
    in
    rev_aux(xs, [])
    end


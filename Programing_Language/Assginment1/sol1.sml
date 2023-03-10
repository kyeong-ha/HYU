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
    end;

fun pi(a: int, b: int, f: (int->int)): int =
    if a=b then f(b)
    else  f(a)*f(pi(a+1, b, f));

fun digits(xs: int): int list =
    let
    fun intToList(xs: int, res: int list): int list = 
        if xs < 10 then xs :: res
        else intToList(xs div 10, (xs mod 10) :: res)
    in
    intToList(xs, [])
    end;

fun additivePersistence(x: int): int list =
    let 
        fun addition(xs: int list, res: int): int list =
            if null xs then res :: []
            else addition(tl xs, (hd xs+res))
    in
    
    addition(digits(x), 0)
    end;

(* additivePersistence(9876); *)


fun digitalRoot(x: int): int =
    let 
        fun addition(xs: int list, res: int): int =
            if null xs then res
            else addition(tl xs, res+(hd xs))
    in
    
    end;
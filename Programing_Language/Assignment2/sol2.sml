(* 1. Simple Eval  *)
datatype expr = NUM of int
              | PLUS of expr * expr
              | MINUS of expr * expr

datatype formula = TRUE
                 | FALSE
                 | NOT of formula
                 | ANDALSO of formula * formula
                 | ORELSE of formula * formula
                 | IMPLY of formula * formula
                 | LESS of expr * expr;

fun IntEval(e: expr): int =
    case e of
      NUM(i) => i
    | PLUS(e1, e2) => IntEval(e1) + IntEval(e2)
    | MINUS(e1, e2) => IntEval(e1) - IntEval(e2)

fun eval(e: formula): bool =
    case e of 
      TRUE => true
    | FALSE => false
    | NOT(e) => if eval(e) = true then false else true
    | ANDALSO(e1, e2) => if eval(e1) = true andalso eval(e2) = true then true else false
    | ORELSE(e1, e2) => if eval(e1) = true orelse eval(e2) = true then true else false
    | IMPLY(e1, e2) => if eval(e1) = false orelse eval(e2) = true then true else false
    | LESS(e1, e2) => if IntEval(e1) < IntEval(e2) then true else false



(* 2. Check MetroMap *)
type name = string
datatype metro = STATION of name
               | AREA of name * metro
               | CONNECT of metro * metro

fun checkMetro(m: metro): bool =
    let
        fun check (n, list) =
            case n of
            STATION nx => 
                let fun hasName(list) =
                    case list of
                          [] => false
                        | hd :: rest => if hd = nx then true else hasName(rest)
                in 
                    hasName(list)
                end
            | AREA(n, mx) => check(mx, n::list)
            | CONNECT(m1, m2) => check(m1, list) andalso check(m2, list)
    in
        check(m, [])
    end



(* 3. Lazy List *)
datatype 'a lazyList = nullList
                     | cons of 'a * (unit -> 'a lazyList)

fun seq(first:int, last:int): int lazyList =
    if first >= last+1
    then nullList
    else cons(first, fn() => seq(first+1, last))

fun infSeq(first:int): int lazyList = 
    cons(first, fn () => infSeq(first+1))

(*****)
fun reverse(xs: 'a list): 'a list = 
    let
        fun rev_aux(ys: 'a list, res: 'a list): 'a list =
            if null ys then res
            else rev_aux(tl ys, (hd ys) :: res)
    in
    rev_aux(xs, [])
    end;
(*****)

fun firstN(lazyListVal:'a lazyList, n:int) =
    let fun addList(lazyListVal, n, list) =
        if n <= 0 then list
        else 
            case lazyListVal of
              nullList => list
            | cons(hd, tl) => addList(tl(), n-1, hd::list)
    in
        reverse(addList(lazyListVal, n, []))
    end

fun Nth(lazyListVal:'a lazyList, n:int) =
    let fun addList(lazyListVal, n) =
        if n <= 0 then NONE
        else
            case lazyListVal of
              nullList => NONE
            | cons(hd,tl) => if n = 1 then SOME hd else addList(tl(), n-1)
    in
        addList(lazyListVal, n)
    end

fun filterMultiples(lazyListVal:int lazyList, n:int) =
    let fun filter(f, lazyListVal) =
        case lazyListVal of
          nullList => nullList
        | cons(hd, tl) => if (f hd) then filter(f, tl())
                          else cons(hd, fn() => filter(f, tl()))
    in
        filter((fn x => x mod n = 0), lazyListVal)
    end

(*****)

fun primes() =
    let fun sieve(lazyListVal) =
        case lazyListVal of
            nullList => nullList
          | cons(hd, tl) => cons(hd, fn() => sieve(filterMultiples(tl(), hd)))
    in
        sieve(infSeq(2))
    end
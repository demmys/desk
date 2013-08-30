public class LazyMethod{
    /*
     * map :: (a -> b) -> [a] -> [b]
     */
    public static Thunk.Array map(Thunk f, Thunk.Array x){
        Thunk[] xs = x.get();
        Thunk.Array a = new Thunk.Array(xs.length);
        for(int i = 0; i < xs.length; i++){
            Thunk fc = f.clone();
            fc.apply(xs[i]);
            a.apply(fc);
        }
        return a;
    }

    /*
     * foldl :: (a -> b -> a) -> a -> [b] -> a
     */
    public static Thunk foldl(Thunk f, Thunk z, Thunk.Array x){
        Thunk[] xs = x.get();
        Thunk fc1, fc2 = z;
        for(int i = 0; i < xs.length; i++){
            fc1 = f.clone();
            fc1.apply(fc2, xs[i]);
            i++;
            if(i == xs.length){
                return fc1;
            }
            fc2 = f.clone();
            fc2.apply(fc1, xs[i]);
        }
        return fc2;
    }

    /*
     * foldr :: (a -> b -> a) -> a -> [b] -> a
     */
    public static Thunk foldr(Thunk f, Thunk z, Thunk.Array x){
        Thunk[] xs = x.get();
        Thunk fc1, fc2 = z;
        for(int i = 0; i < xs.length; i++){
            fc1 = f.clone();
            fc1.apply(xs[i], fc2);
            i++;
            if(i == xs.length){
                return fc1;
            }
            fc2 = f.clone();
            fc2.apply(xs[i], fc1);
        }
        return fc2;
    }

    /*
     * zipWith :: (a -> b -> c) -> [a] -> [b] -> [c]
     */
    public static Thunk.Array zipWith(Thunk f, Thunk.Array x, Thunk.Array y){
        Thunk[] xs = x.get();
        Thunk[] ys = y.get();
        int min = Math.min(xs.length, ys.length);
        Thunk.Array a = new Thunk.Array(min);
        for(int i = 0; i < min; i++){
            Thunk fc = f.clone();
            fc.apply(xs[i], ys[i]);
            a.apply(fc);
        }
        return a;
    }

    /*
     * range :: Int -> Int -> [Int]
     */
    public static Thunk.Array range(Thunk s, Thunk e){
        int ns = s.eval().getInt();
        int ne = e.eval().getInt();
        Thunk.Array a = new Thunk.Array(ne - ns + 1);
        while(ns <= ne){
            a.apply(new LazyValue(ns++));
        }
        return a;
    }
}

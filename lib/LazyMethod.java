public class LazyMethod{
    /*
     * map :: (a -> b) -> [a] -> [b]
     */
    public static Thunk[] map(Thunk f, Thunk[] xs){
        for(int i = 0; i < xs.length; i++){
            Thunk fc = f.clone();
            fc.apply(xs[i]);
            xs[i] = fc;
        }
        return xs;
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

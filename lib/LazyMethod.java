public class LazyMethod{
    /*
     * amap :: (a -> b) -> {a} -> {b}
     */
    public static LazyStructure.Array amap(Thunk f, LazyStructure.Array x){
        Thunk[] xs = x.get();
        LazyStructure.Array a = new LazyStructure.Array(xs.length);
        for(int i = 0; i < xs.length; i++){
            Thunk fc = f.clone();
            fc.apply(xs[i]);
            a.apply(fc);
        }
        return a;
    }

    /*
     * afoldl :: (a -> b -> a) -> a -> {b} -> a
     */
    public static Thunk afoldl(Thunk f, Thunk z, LazyStructure.Array x){
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
     * afoldr :: (a -> b -> a) -> a -> {b} -> a
     */
    public static Thunk afoldr(Thunk f, Thunk z, LazyStructure.Array x){
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
     * azipWith :: (a -> b -> c) -> {a} -> {b} -> {c}
     */
    public static LazyStructure.Array azipWith(Thunk f, LazyStructure.Array x, LazyStructure.Array y){
        Thunk[] xs = x.get();
        Thunk[] ys = y.get();
        int min = Math.min(xs.length, ys.length);
        LazyStructure.Array a = new LazyStructure.Array(min);
        for(int i = 0; i < min; i++){
            Thunk fc = f.clone();
            fc.apply(xs[i], ys[i]);
            a.apply(fc);
        }
        return a;
    }

    /*
     * arange :: Int -> Int -> {Int}
     */
    public static LazyStructure.Array arange(Thunk s, Thunk e){
        int ns = s.eval().getInt();
        int ne = e.eval().getInt();
        LazyStructure.Array a = new LazyStructure.Array(ne - ns + 1);
        while(ns <= ne){
            a.apply(new LazyValue(ns++));
        }
        return a;
    }
}

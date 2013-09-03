public class Operator{
    /*
     * add :: Int -> Int -> Int
     */
    public static class Add extends Function{
        public Add(Function... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                return new LazyValue(n1 + n2);
            }
    }

    /*
     * sub :: Int -> Int -> Int
     */
    public static class Sub extends Function{
        public Sub(Function... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                return new LazyValue(n1 - n2);
            }
    }

    /*
     * mul :: Int -> Int -> Int
     */
    public static class Mul extends Function{
        public Mul(Function... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                return new LazyValue(n1 * n2);
            }
    }

    /*
     * div :: Int -> Int -> Int
     */
    public static class Div extends Function{
        public Div(Function... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                return new LazyValue(n1 / n2);
            }
    }

    /*
     * mod :: Int -> Int -> Int
     */
    public static class Mod extends Function{
        public Mod(Function... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                return new LazyValue(n1 % n2);
            }
    }
}

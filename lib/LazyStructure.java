public class LazyStructure{
    /*
     * array :: {a}
     */
    public static class Array extends Thunk{
        public Array(int length, Thunk... args){
            super(length, args);
        }

        public Thunk[] get(){
            return args;
        }

        @Override
            public LazyValue eval(){
                return null;
            }
    }

    /*
     * cons :: a -> [a] -> [a]
     */
    public static class Cons extends Thunk{
        public Cons(Thunk... args){
            super(2, args);
        }

        public Thunk car(){
            return args[0];
        }

        public Thunk cdr(){
            return args[1];
        }

        @Override
            public LazyValue eval(){
                return null;
            }
    }

    /*
     * nil :: []
     */
    public static class Nil extends Thunk{
        public Nil(){
            super();
        }

        @Override
            public LazyValue eval(){
                return null;
            }
    }
}

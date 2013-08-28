public class Fibonacci{
    private static class Fib extends Thunk{
        public Fib(Thunk... args){
            super(1, args);
        }

        @Override
            public LazyValue eval(){
                int n = this.args[0].eval().getInt();
                if(n < 3){
                    return new LazyValue(1);
                }
                int a1 = new Fib(new LazyValue(n - 2)).eval().getInt();
                int a2 = new Fib(new LazyValue(n - 1)).eval().getInt();
                return new LazyValue(a1 + a2);
            }
    }

    public static void main(String[] args){
        int arg = Integer.parseInt(args[0]);
        Thunk f = new Fib(new LazyValue(arg));
        int res = f.eval().getInt();
        System.out.println(res);
    }
}

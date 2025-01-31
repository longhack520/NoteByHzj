JavaCompletableFuture<br />日常开发中，都会用到线程池，一般会用`execute()`和`submit()`方法提交任务。但是当用过`CompletableFuture`之后，就会发现以前的线程池处理任务有多难用，功能有多简陋，`CompletableFuture`又是多么简洁优雅。<br />要知道`CompletableFuture`已经随着Java8发布7年了，还没有过它就有点说不过去了。<br />深入浅出`CompletableFuture`实用教程。
<a name="brFzJ"></a>
## 1、使用线程池处理任务
```java
/**
 * @apiNote 线程池使用示例
 */
public class ThreadDemo {

	public static void main(String[] args) {
		// 1. 创建线程池
		ExecutorService executorService = Executors.newFixedThreadPool(3);

		List<Integer> list = Arrays.asList(1, 2, 3);
		List<Future<String>> futures = new ArrayList<>();
		for (Integer key : list) {
			// 2. 提交任务
			Future<String> future = executorService.submit(() -> {
				// 睡眠一秒，模仿处理过程
				Thread.sleep(1000L);
				return "结果" + key;
			});
			futures.add(future);
		}

		// 3. 获取结果
		for (Future<String> future : futures) {
			try {
				String result = future.get();
				System.out.println(result);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		executorService.shutdown();
	}

}
```
输出结果：
```java
结果1
结果2
结果3
```
一般大家都会这样使用线程池，但是有没有思考过这样使用有没有什么问题？<br />可以发现两个比较严重的问题：

1. **获取结果时，调用的**`**future.get()**`**方法，会阻塞当前线程，直到返回结果，大大降低性能**
2. **有一半的代码在写怎么使用线程，其实不应该关心怎么使用线程，更应该关注任务的处理**

有没有具体的优化方案呢？当然有了，那就是主角`CompletableFuture`
<a name="lgDh1"></a>
## 2、使用`CompletableFuture`重构任务处理
看一下使用`CompletableFuture`改造后代码：
```java
/**
 * @apiNote CompletableFuture使用示例
 */
public class ThreadDemo {

	public static void main(String[] args) {
		// 1. 创建线程池
		ExecutorService executorService = Executors.newFixedThreadPool(3);

		List<Integer> list = Arrays.asList(1, 2, 3);
		for (Integer key : list) {
			// 2. 提交任务
			CompletableFuture.supplyAsync(() -> {
				// 睡眠一秒，模仿处理过程
				try {
					Thread.sleep(1000L);
				} catch (InterruptedException e) {
				}
				return "结果" + key;
			}, executorService).whenCompleteAsync((result, exception) -> {
				// 3. 获取结果
				System.out.println(result);
			});;
		}

		executorService.shutdown();
		// 由于whenCompleteAsync获取结果的方法是异步的，所以要阻塞当前线程才能输出结果
		try {
			Thread.sleep(2000L);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

}
```
输出结果：
```java
结果1
结果2
结果3
```
代码中使用了`CompletableFuture`的两个方法，`supplyAsync()`方法作用是提交异步任务，有两个传参，任务和自定义线程池。<br />`whenCompleteAsync()`方法作用是异步获取结果，也有两个传参，结果和异常信息。<br />代码经过`CompletableFuture`改造后，是多么的简洁优雅。<br />提交任务也不用再关心线程池是怎么使用了，获取结果也不用再阻塞当前线程了。<br />如果比较倔强，还想同步获取结果，可以使用`whenComplete()`方法，或者单独调用`join()`方法。<br />`join()`方法配合Stream流是这样用的：
```java
/**
 * @apiNote CompletableFuture使用示例
 */
public class ThreadDemo {

	public static void main(String[] args) {
		// 1. 创建线程池
		ExecutorService executorService = Executors.newFixedThreadPool(3);

		List<Integer> list = Arrays.asList(1, 2, 3);
		// 2. 提交任务
		List<String> results = list.stream().map(key ->
												 CompletableFuture.supplyAsync(() -> {
													 // 睡眠一秒，模仿处理过程
													 try {
														 Thread.sleep(1000L);
													 } catch (InterruptedException e) {
													 }
													 return "结果" + key;
												 }, executorService))
			.map(CompletableFuture::join).collect(Collectors.toList());

		executorService.shutdown();
		// 3. 获取结果
		System.out.println(results);

	}

}
```
输出结果：
```java
[结果1,结果2,结果3]
```
多么的简洁优雅啊！原来`executorService.submit()`这种使用线程池的方式，可以彻底丢掉了。
<a name="y11Rz"></a>
## 3、`CompletableFuture`更多妙用
<a name="Yhgxx"></a>
### 3.1 等待所有任务执行完成
如果让实现等待所有任务线程执行完成，再进行下一步操作，会怎么做？<br />一般会使用 线程池+CountDownLatch，像下面这样：
```java
/**
 * @apiNote 线程池和CountDownLatch使用示例
 */
public class ThreadDemo {

	public static void main(String[] args) {
		// 1. 创建线程池
		ExecutorService executorService = Executors.newFixedThreadPool(3);

		List<Integer> list = Arrays.asList(1, 2, 3);
		CountDownLatch countDownLatch = new CountDownLatch(list.size());
		for (Integer key : list) {
			// 2. 提交任务
			executorService.execute(() -> {
				// 睡眠一秒，模仿处理过程
				try {
					Thread.sleep(1000L);
				} catch (InterruptedException e) {
				}
				System.out.println("结果" + key);
				countDownLatch.countDown();
			});
		}

		executorService.shutdown();
		// 3. 阻塞等待所有任务执行完成
		try {
			countDownLatch.await();
		} catch (InterruptedException e) {
		}
	}

}
```
输出结果：
```java
结果2
结果3
结果1
```
Low不Low？十年前可以这样写，Java8都已经发布7年了，还不会用Java8的写法？看一下使用`CompletableFuture`是怎么重构的：
```java
/**
 * @apiNote CompletableFuture.allOf()方法使用示例
 */
public class ThreadDemo {

	public static void main(String[] args) {
		// 1. 创建线程池
		ExecutorService executorService = Executors.newFixedThreadPool(3);

		List<Integer> list = Arrays.asList(1, 2, 3);
		// 2. 提交任务，并调用join()阻塞等待所有任务执行完成
		CompletableFuture
			.allOf(
			list.stream().map(key ->
							  CompletableFuture.runAsync(() -> {
								  // 睡眠一秒，模仿处理过程
								  try {
									  Thread.sleep(1000L);
								  } catch (InterruptedException e) {
								  }
								  System.out.println("结果" + key);
							  }, executorService))
			.toArray(CompletableFuture[]::new))
			.join();
		executorService.shutdown();
	}

}
```
输出结果：
```java
结果3
结果1
结果2
```
代码看着有点乱，其实逻辑很清晰。

1. **遍历list集合，提交**`**CompletableFuture**`**任务，把结果转换成数组**
2. **再把数组放到**`**CompletableFuture**`**的**`**allOf()**`**方法里面**
3. **最后调用**`**join()**`**方法阻塞等待所有任务执行完成**

`CompletableFuture`的`allOf()`方法的作用就是，等待所有任务处理完成。<br />这样写是不是简洁优雅了许多？
<a name="sMiDq"></a>
### 3.2 任何一个任务处理完成就返回
如果要实现这样一个需求，往线程池提交一批任务，只要有其中一个任务处理完成就返回。<br />该怎么做？如果手动实现这个逻辑的话，代码肯定复杂且低效，有了`CompletableFuture`就非常简单了，只需调用`anyOf()`方法就行了。
```java
/**
 * @apiNote CompletableFuture.anyOf()方法使用示例
 */
public class ThreadDemo {

	public static void main(String[] args) {
		// 1. 创建线程池
		ExecutorService executorService = Executors.newFixedThreadPool(3);

		List<Integer> list = Arrays.asList(1, 2, 3);
		long start = System.currentTimeMillis();
		// 2. 提交任务
		CompletableFuture<Object> completableFuture = CompletableFuture
			.anyOf(
			list.stream().map(key ->
							  CompletableFuture.supplyAsync(() -> {
								  // 睡眠一秒，模仿处理过程
								  try {
									  Thread.sleep(1000L);
								  } catch (InterruptedException e) {
								  }
								  return "结果" + key;
							  }, executorService))
			.toArray(CompletableFuture[]::new));
		executorService.shutdown();

		// 3. 获取结果
		System.out.println(completableFuture.join());
	}

}
```
输出结果：
```java
结果3
```
一切都是那么简单优雅。
<a name="Jvq7J"></a>
### 3.3 一个线程执行完成，交给另一个线程接着执行
有这么一个需求：<br />一个线程处理完成，把处理的结果交给另一个线程继续处理，怎么实现？<br />是不是想到了一堆工具，线程池、`CountDownLatch`、`Semaphore`、`ReentrantLock`、Synchronized，该怎么进行组合使用呢？AB组合还是BC组合？<br />别瞎想了，写的肯定没有`CompletableFuture`好用，看一下`CompletableFuture`是怎么用的：
```java
/**
 * @apiNote CompletableFuture线程接力处理示例
 */
public class ThreadDemo {

	public static void main(String[] args) {
		// 1. 创建线程池
		ExecutorService executorService = Executors.newFixedThreadPool(2);

		// 2. 提交任务，并调用join()阻塞等待任务执行完成
		String result2 = CompletableFuture.supplyAsync(() -> {
			// 睡眠一秒，模仿处理过程
			try {
				Thread.sleep(1000L);
			} catch (InterruptedException e) {
			}
			return "结果1";
		}, executorService).thenApplyAsync(result1 -> {
			// 睡眠一秒，模仿处理过程
			try {
				Thread.sleep(1000L);
			} catch (InterruptedException e) {
			}
			return result1 + "结果2";
		}, executorService).join();

		executorService.shutdown();
		// 3. 获取结果
		System.out.println(result2);
	}

}
```
输出结果：
```java
结果1结果2
```
代码主要用到了`CompletableFuture`的`thenApplyAsync()`方法，作用就是异步处理上一个线程的结果。<br />是不是太方便了？<br />这么好用的`CompletableFuture`还有没有其他功能？当然有。
<a name="x0W2B"></a>
## 4、CompletableFuture常用API
<a name="uRuvv"></a>
### 4.1 CompletableFuture常用API说明
<a name="AFNkD"></a>
#### 提交任务
`**supplyAsync**`**`**runAsync**`
<a name="MLSrY"></a>
#### 接力处理
`**thenRun**`** **`**thenRunAsync**`**`**thenAccept**`** **`**thenAcceptAsync**`**`**thenApply**`** **`**thenApplyAsync**`**`**handle**`** **`**handleAsync**`**`**applyToEither**`** **`**applyToEitherAsync**`**`**acceptEither**`** **`**acceptEitherAsync**`**`**runAfterEither**`** **`**runAfterEitherAsync**`**`**thenCombine**`** **`**thenCombineAsync**`**`**thenAcceptBoth**`** **`**thenAcceptBothAsync**`<br />API太多，有点眼花缭乱，很容易分类。<br />带`run`的方法，无入参，无返回值。<br />带`accept`的方法，有入参，无返回值。<br />带`supply`的方法，无入参，有返回值。<br />带`apply`的方法，有入参，有返回值。<br />带`handle`的方法，有入参，有返回值，并且带异常处理。<br />以`Async`结尾的方法，都是异步的，否则是同步的。<br />以`Either`结尾的方法，只需完成任意一个。<br />以`Both`/`Combine`结尾的方法，必须所有都完成。
<a name="HMR8q"></a>
#### 获取结果
`**join**`** 阻塞等待，不会抛异常<br />**`**get**`** 阻塞等待，会抛异常<br />**`**complete(T value)**`** 不阻塞，如果任务已完成，返回处理结果。如果没完成，则返回传参value。<br />**`**completeExceptionally(Throwable ex)**`** 不阻塞，如果任务已完成，返回处理结果。如果没完成，抛异常。**
<a name="HTiQX"></a>
### 4.2 `CompletableFuture`常用API使用示例
用最常见的煮饭来举例：
<a name="f0Aum"></a>
#### 1、`then`、`handle`方法使用示例
```java
/**
* then、handle方法使用示例
*/
public class ThreadDemo {

	public static void main(String[] args) {
		CompletableFuture<String> completableFuture = CompletableFuture.supplyAsync(() -> {
			System.out.println("1. 开始淘米");
			return "2. 淘米完成";
		}).thenApplyAsync(result -> {
			System.out.println(result);
			System.out.println("3. 开始煮饭");
			// 生成一个1~10的随机数
			if (RandomUtils.nextInt(1, 10) > 5) {
				throw new RuntimeException("4. 电饭煲坏了，煮不了");
			}
			return "4. 煮饭完成";
		}).handleAsync((result, exception) -> {
			if (exception != null) {
				System.out.println(exception.getMessage());
				return "5. 今天没饭吃";
			} else {
				System.out.println(result);
				return "5. 开始吃饭";
			}
		});

		try {
			String result = completableFuture.get();
			System.out.println(result);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
```
输出结果可能是：
```
1. 开始淘米
2. 淘米完成
3. 开始煮饭
4. 煮饭完成
5. 开始吃饭
```
也可能是：
```
1. 开始淘米
2. 淘米完成
3. 开始煮饭
java.lang.RuntimeException: 4. 电饭煲坏了，煮不了
5. 今天没饭吃
```
<a name="UlpzG"></a>
#### 2、`complete`方法使用示例
```java
public class ThreadDemo {

	public static void main(String[] args) {
		CompletableFuture<String> completableFuture = CompletableFuture.supplyAsync(() -> {
			return "饭做好了";
		});

		//try {
		//    Thread.sleep(1L);
		//} catch (InterruptedException e) {
		//}

		completableFuture.complete("饭还没做好，我点外卖了");
		System.out.println(completableFuture.join());
	}

}
```
输出结果：
```java
饭还没做好，我点外卖了
```
如果把注释的`sleep()`方法放开，输出结果就是:
```java
饭做好了
```
<a name="AxPnf"></a>
#### 3、either方法使用示例
```java
public class ThreadDemo {

	public static void main(String[] args) {
		CompletableFuture<String> meal = CompletableFuture.supplyAsync(() -> {
			return "饭做好了";
		});
		CompletableFuture<String> outMeal = CompletableFuture.supplyAsync(() -> {
			return "外卖到了";
		});

		// 饭先做好，就吃饭。外卖先到，就吃外卖。就是这么任性。
		CompletableFuture<String> completableFuture = meal.applyToEither(outMeal, myMeal -> {
			return myMeal;
		});

		System.out.println(completableFuture.join());
	}

}
```
输出结果可能是：
```sql
饭做好了
```
也可能是：
```sql
外卖到了
```
学会了吗？开发中赶快用起来吧！

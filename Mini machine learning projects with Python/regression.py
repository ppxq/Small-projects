"""
This is a logistic regression solution for image classfication. 
Data is from MNIST, with hand written numbers.
Model is trained with gradient descent method
"""


from tensorflow.examples.tutorials.mnist import input_data

import tensorflow as tf

#import dataset,transform the output to one hot vector
mnist = input_data.read_data_sets(".", one_hot=True)

#allocate memory
x = tf.placeholder(tf.float32, [None, 784])
W = tf.Variable(tf.zeros([784, 10]))
b = tf.Variable(tf.zeros([10]))
y = tf.matmul(x, W) + b
y_ = tf.placeholder(tf.float32, [None, 10])

#define loss function as softmax cross entropy with penalty
cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(labels=y_, logits=y))
train_step = tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)

#initialization for the hyper parameters
sess = tf.InteractiveSession()  #cuda startup noise here
tf.global_variables_initializer().run()

#begine to train 
for _ in range(1000):
  batch_xs, batch_ys = mnist.train.next_batch(100)
  sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys})

#test the model
correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print(sess.run(accuracy, feed_dict={x: mnist.test.images, y_: mnist.test.labels}))
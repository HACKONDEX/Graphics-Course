# Graphics-Course
MIPT 3D visualization course, semester 6 - 2022-/-Spring

**Student:** Сагателян Акоп - Б05-923

**Theoretical tasks**
-----------------------------------------------------------------

__Решение Задачи 1__
==============================

Если точка $$А(a,b)$$ совподает с точкой $$(0,0)$$, то матрица поворота выглядит вот так

$$
    R = \left(\begin{array}{cc}
    \cos\varphi & \sin\varphi \\
    -\sin\varphi & \cos\varphi\end{array}\right)
$$

Давайте введум третью координату $w$ как мы уже делали, и будем использовать перенос. Сначала перенесем плоскость так, чтобы точка $A$ совподала с $(0,0)$ а потом повернем используя расширенную матрицу $R$. Останется лишь сдвинуть плоскость обратнаным переносом. В итоге получим, что поворот был вокруг точки $A$.

$
                        R(Расширенный) = \left(\begin{array}{ccc}
    \cos\varphi & \sin\varphi & 0 \\
    -\sin\varphi & \cos\varphi & 0 \\
    0 & 0 & 1\end{array}\right), Точка\:X = \left(\begin{array}{c} x\\ y\\ 1\end{array}\right)
$

Матрица $S$  будет переносом в $(0,0)$, a $S^{'}$ будет обратным переносом. Тогда,
$S^{'} = S^{-1},\:\:
    S = \left(\begin{array}{ccc} 
    1 & 0 & -a \\ 
    0 & 1 & -b \\ 
    0 & 0 & 1\end{array}\right), \: S^{'} = \left(\begin{array}{ccc} 
                                            1 & 0 & a\\ 
                                            0 & 1 & b\\ 
                                            0 & 0 & 1\end{array}\right)$
                                            
Вспоминаем,что применять матрицы к точке надо с право на лево и получаем

$T = \left(\begin{array}{ccc} 1 & 0 & a\\ 0 & 1 & b\\ 0 & 0 & 1\end{array}\right) * \left(\begin{array}{ccc} \cos\varphi & \sin\varphi & 0 \\ -\sin\varphi & \cos\varphi & 0 \\ 0 & 0 & 1\end{array}\right) * \left(\begin{array}{ccc} 1 & 0 & -a \\ 0 & 1 & -b \\ 0 & 0 & 1\end{array}\right) = \left(\begin{array}{ccc} \cos\varphi & \sin\varphi & a(1-\cos\varphi)-b\sin\varphi \\ -\sin\varphi & \cos\varphi & a\sin\varphi+b(1-\cos\varphi) \\ 0 & 0 & 1 \end{array}\right)$


__Решение Задачи 2__
==============================

Для получения итогого поворота вокруг прямой мы сначала перенесем пространство так, чтобы точка $A$ совподал с точкой $(0,0,0)$ потом мы сделаем смещение, чтобы прямая $L$ совподала с осью координат $z$ и тогда, можно будет применить знакомый нам поворот в плоскости $xOy$.

Матрица переноса нам уже знакома, тут она будет четырехмерная 

$S = \left(\begin{array}{cccc} 1 & 0 & 0 & -a \\ 0 & 1 & 0 & -b \\ 0 & 0 & 1 & -c \\ 0 & 0 & 0 & 1 \end{array}\right),$ слудовательно обратный перенос $S^{'} = \left(\begin{array}{cccc} 1 & 0 & 0 & a \\ 0 & 1 & 0 & b \\ 0 & 0 & 1 & c \\ 0 & 0 & 0 & 1 \end{array}\right)$

Совмещение $L$ с осью $z$ это соответсвенно два поворота, один вокруг оси $x$ на угол $\psi$ и второй вокруг $y$ на угол $\theta$

Получаем $R_x(\psi) = \left(\begin{array}{cccc} 1 & 0 & 0 & 0 \\ 0 & \frac{n}{\sqrt{n^2 + m^2}} & -\frac{m}{\sqrt{n^2 + m^2}} & 0 \\ 0 & \frac{m}{\sqrt{n^2 + m^2}} & \frac{n}{\sqrt{n^2 + m^2}} & 0 \\ 0 & 0 & 0 & 1 \end{array}\right)$ 

далее пользуясь тем, что $n^2 + m^2 + l^2 = 1$ получам матрицу $R_y(\theta) = \left(\begin{array}{cccc} \sqrt{n^2 + m^2} & 0 & -l & 0 \\ 0 & 1 & 0 & 0 \\ l & 0 & \sqrt{n^2 + m^2} & 0 \\ 0 & 0 & 0 & 1 \end{array}\right)$

Так как это матрицы поаоротов, то у них точно существуют обратные, но выписывать их мы не будем. Итоговый поворот будет выглядеть так.

$R_L(\varphi) = S^{'} * R_x^{-1}(\psi) * R_y^{-1}(\theta) * \left(\begin{array}{cccc} \cos\varphi & \sin\varphi & 0 & 0 \\ -\sin\varphi & \cos\varphi & 0 & 0 \\ 0 & 0 & 1 & 0 \\ 0 & 0 & 0 & 1 \end{array}\right) * R_y(\theta) * R_x(\psi) * S$

__Решение Задачи 2__
==============================

Возьмём кватернионы соответствующие поворотам

$\Lambda_1 = \cos\frac{\varphi}{2} + e_x\sin\frac{\varphi}{2}, \: \Lambda_2 = \cos\frac{\varphi}{2} + e_y\sin\frac{\varphi}{2}, \: \varphi = \frac{\pi}{2}, \: e_x = \left(\begin{array}{ccc} 1 \\ 0 \\ 0 \end{array}\right), \: e_y = \left(\begin{array}{c} 0 \\ 0 \\ 1 \end{array}\right) \: \Rightarrow \: \Lambda_1 = \frac{1}{\sqrt{2}}(1 + j), \: \Lambda_2 = \frac{1}{\sqrt{2}}(1 + i)$

Результирующий поворот будет их произведением в группе кватернионов

$\Lambda = \Lambda_2 \cdot \Lambda_1 = \frac{1}{2}(1 + j)(1 + i) = \frac{1}{2}(1 + i + j - k) = \frac{1}{2} + \frac{1}{2}(i + j - k) = \frac{1}{2} + \frac{\sqrt{3}}{2} \left(\begin{array}{ccc} 1 \\ 1 \\ -1 \end{array}\right) \Rightarrow $ 

$\Rightarrow$ итоговый угол поворота будет $\varphi = \frac{2\pi}{3}$, а векторная часть $\xi = \left(\begin{array}{ccc} = \frac{1}{\sqrt{3}} \\ \frac{1}{\sqrt{3}} \\ -\frac{1}{\sqrt{3}} \end{array}\right)$ 
# 例题

渲染网站：https://wavedrom.com/editor.html

```wavedorm
{
  signal: [
  	{
      name: 'cs', 
      wave:"10...............................1.",
      period: "0.5"
    },
    
  	{
      name: 'clk', 
      wave:"0.10101010101010101010101010101010.",
      period: "0.5"
    },
    
  	{
      name: 'mosi', 
      wave:"x1.0.1...0.1...0.xxxxxxxxxxxxxxxxxx",
      period: "0.5"
    },
    
  	{
      name: 'miso', 
      wave:"xxxxxxxxxxxxxxxxx0.1.......0.....xx",
      period: "0.5"
    },
    
  	{
      name: '主机发出数据', 
      wave:"x=.=.=.=.=.=.=.=.xxxxxxxxxxxxxxxxxx",
      data:[1, 0, 1, 1, 0, 1, 1, 0],
      period: "0.5"
    },
    
  	{
      name: '从机响应数据', 
      wave:"xxxxxxxxxxxxxxxxx=.=.=.=.=.=.=.=.xx",
      data:[0, 1, 1, 1, 1, 0, 0, 0],
      period: "0.5"
    },
]}

```


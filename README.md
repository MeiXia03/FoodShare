#放F盘下启动

#sql表
-- 用户表
CREATE TABLE IF NOT EXISTS users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    avatar_path TEXT DEFAULT 'default_avatar.png',
    signature TEXT,
    is_admin BOOLEAN DEFAULT 0,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- 菜谱表
CREATE TABLE IF NOT EXISTS recipes (
    recipe_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    title TEXT NOT NULL,
    content TEXT NOT NULL,
    category TEXT, -- 如'川菜'
    type TEXT, -- 如'海鲜类'
    video_path TEXT,
    likes INTEGER DEFAULT 0,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);

-- 评论表
CREATE TABLE IF NOT EXISTS comments (
    comment_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    recipe_id INTEGER NOT NULL,
    content TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (recipe_id) REFERENCES recipes(recipe_id)
);

-- 黑名单表
CREATE TABLE IF NOT EXISTS blacklist (
    block_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    admin_id INTEGER NOT NULL,
    reason TEXT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (admin_id) REFERENCES users(user_id)
);

-- 反馈表
CREATE TABLE IF NOT EXISTS feedback (
    feedback_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    content TEXT NOT NULL,
    status TEXT DEFAULT 'pending', -- pending/resolved
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);

#设计需求
设计这个平台主要是为了方便广大用户更多的了解或分享身边的美食，或者满足那些想要寻求更多美食的用户，通过大数据分析用户对美食系统的具体需求和期望，以及对程序使用体验的了解。或是为了满足那些喜欢美食背后的故事，想要更多的了解美食的起源与传承的人。
搜索功能：用户可以通过搜索来了解某种食谱或是烹饪技巧视频，方便用户快速寻找自己想要了解的菜谱。
分类功能：用户可以通过自己的喜好来选择食物的种类：例如海鲜类，畜肉类等。或是选择菜系：粤菜，川菜等。
上传功能：用户可以上传有关美食的体验或是美食背后的故事，或是创建有关的分享或烹饪技巧视频。
社区功能：用户与用户之间可以通过评论，好友等方式联系，方便互相交流美食心得与体会，或是一同探寻美食背后的含义。
主界面：设计友好的引导功能，方便用户第一次上手体验产品。确保信息显示清晰，操作简单直观，提升用户体验。
点赞系统：被大多数用户所推荐的视频会被有限显示，优质视频会被推荐到主页中。
个人信息系统：允许用户更换头像，个性签名等个人信息，方便用户之间进行交流，优质用户会被推荐到首页分享。
管理员系统：管理员可以管理社区中用户的推文，将不合法的推文和被举报的用户拉入黑名单，维护社区的环境。或是审核视频的合规性，食谱的准确性，评论区的适当性。
Bug反馈系统：通过联系技术支持人员，提供问题，投诉或是反馈，从而针对性的提供技术支持和客户服务。并帮助用户解决问题和功能疑问。
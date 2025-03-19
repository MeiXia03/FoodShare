/*
 Navicat Premium Data Transfer

 Source Server         : food
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 18/03/2025 10:07:42
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for blacklist
-- 黑名单表：存储被封禁用户的信息
-- ----------------------------
DROP TABLE IF EXISTS "blacklist";
CREATE TABLE "blacklist" (
  "block_id" INTEGER PRIMARY KEY AUTOINCREMENT,         -- 封禁记录ID（主键，自增长）
  "user_id" INTEGER NOT NULL,                          -- 被封锁用户ID（关联users表）
  "admin_id" INTEGER NOT NULL,                         -- 执行封禁操作的管理员ID（关联users表）
  "reason" TEXT,                                       -- 封禁原因
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,     -- 封禁时间（默认当前时间戳）
  FOREIGN KEY ("user_id") REFERENCES "users" ("user_id") ON DELETE NO ACTION ON UPDATE NO ACTION,
  FOREIGN KEY ("admin_id") REFERENCES "users" ("user_id") ON DELETE NO ACTION ON UPDATE NO ACTION
);

-- ----------------------------
-- Table structure for comments
-- 评论表：存储用户对食谱的评论信息
-- ----------------------------
DROP TABLE IF EXISTS "comments";
CREATE TABLE "comments" (
  "comment_id" INTEGER PRIMARY KEY AUTOINCREMENT,       -- 评论ID（主键，自增长）
  "user_id" INTEGER NOT NULL,                          -- 发表评论的用户ID（关联users表）
  "recipe_id" INTEGER NOT NULL,                        -- 被评论的食谱ID（关联recipes表）
  "content" TEXT NOT NULL,                             -- 评论内容
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,     -- 评论时间（默认当前时间戳）
  FOREIGN KEY ("user_id") REFERENCES "users" ("user_id") ON DELETE NO ACTION ON UPDATE NO ACTION,
  FOREIGN KEY ("recipe_id") REFERENCES "recipes" ("recipe_id") ON DELETE NO ACTION ON UPDATE NO ACTION
);

-- ----------------------------
-- Table structure for feedback
-- 用户反馈表：存储用户提交的反馈信息
-- ----------------------------
DROP TABLE IF EXISTS "feedback";
CREATE TABLE "feedback" (
  "feedback_id" INTEGER PRIMARY KEY AUTOINCREMENT,      -- 反馈ID（主键，自增长）
  "user_id" INTEGER NOT NULL,                          -- 提交反馈的用户ID（关联users表）
  "content" TEXT NOT NULL,                             -- 反馈内容
  "status" TEXT DEFAULT 'pending',                     -- 反馈状态（默认待处理）
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,     -- 提交时间（默认当前时间戳）
  FOREIGN KEY ("user_id") REFERENCES "users" ("user_id") ON DELETE NO ACTION ON UPDATE NO ACTION
);

-- ----------------------------
-- Table structure for recipes
-- 食谱表：存储用户发布的食谱信息
-- ----------------------------
DROP TABLE IF EXISTS "recipes";
CREATE TABLE "recipes" (
  "recipe_id" INTEGER PRIMARY KEY AUTOINCREMENT,        -- 食谱ID（主键，自增长）
  "user_id" INTEGER NOT NULL,                          -- 发布食谱的用户ID（关联users表）
  "title" TEXT NOT NULL,                               -- 食谱标题
  "content" TEXT NOT NULL,                             -- 食谱详细内容/步骤
  "category" TEXT,                                     -- 分类（如中餐、西餐）
  "type" TEXT,                                         -- 类型（如素食、荤食）
  "video_path" TEXT,                                   -- 关联视频的存储路径
  "likes" INTEGER DEFAULT 0,                           -- 点赞数（默认0）
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,     -- 创建时间（默认当前时间戳）
  FOREIGN KEY ("user_id") REFERENCES "users" ("user_id") ON DELETE NO ACTION ON UPDATE NO ACTION
);

-- ----------------------------
-- Table structure for sqlite_sequence
-- SQLite系统表：用于自增主键的序列记录（自动生成，无需手动维护）
-- ----------------------------
DROP TABLE IF EXISTS "sqlite_sequence";
CREATE TABLE "sqlite_sequence" (
  "name",  -- 表名
  "seq"    -- 当前自增序列值
);

-- ----------------------------
-- Table structure for users
-- 用户表：存储所有用户信息，包含普通用户和管理员
-- ----------------------------
DROP TABLE IF EXISTS "users";
CREATE TABLE "users" (
  "user_id" INTEGER PRIMARY KEY AUTOINCREMENT,          -- 用户ID（主键，自增长）
  "username" TEXT NOT NULL,                            -- 用户名（唯一）
  "password" TEXT NOT NULL,                            -- 密码（加密存储）
  "avatar_path" TEXT DEFAULT 'default_avatar.png',     -- 头像路径（默认使用默认头像）
  "signature" TEXT,                                    -- 个性签名
  "is_admin" BOOLEAN DEFAULT 0,                        -- 管理员标识（0-普通用户，1-管理员）
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,     -- 注册时间（默认当前时间戳）
  UNIQUE ("username" ASC)                              -- 用户名唯一约束
);

-- ----------------------------
-- Table structure for friends
-- 好友关系表：存储用户之间的好友关系
-- ----------------------------
DROP TABLE IF EXISTS "friends";
CREATE TABLE "friends" (
  "friend_id" INTEGER PRIMARY KEY AUTOINCREMENT,       -- 好友关系ID（主键，自增长）
  "user_id" INTEGER NOT NULL,                         -- 用户ID（关联users表）
  "friend_user_id" INTEGER NOT NULL,                  -- 好友的用户ID（关联users表）
  "status" TEXT DEFAULT 'pending',                    -- 好友关系状态（pending-待确认，accepted-已接受，blocked-已屏蔽）
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,    -- 好友关系创建时间（默认当前时间戳）
  FOREIGN KEY ("user_id") REFERENCES "users" ("user_id") ON DELETE CASCADE ON UPDATE NO ACTION,
  FOREIGN KEY ("friend_user_id") REFERENCES "users" ("user_id") ON DELETE CASCADE ON UPDATE NO ACTION,
  UNIQUE ("user_id", "friend_user_id")                -- 用户与好友的关系唯一
);

-- ----------------------------
-- 消息记录表
-- ----------------------------
DROP TABLE IF EXISTS "messages";
CREATE TABLE "messages" (
  "message_id" INTEGER PRIMARY KEY AUTOINCREMENT,     -- 消息ID（主键，自增长）
  "sender_id" INTEGER NOT NULL,                       -- 发送者用户ID（关联users表）
  "receiver_id" INTEGER NOT NULL,                     -- 接收者用户ID（关联users表）
  "content" TEXT NOT NULL,                            -- 消息内容
  "is_read" BOOLEAN DEFAULT 0,                        -- 是否已读（0-未读，1-已读）
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,    -- 消息发送时间（默认当前时间戳）
  FOREIGN KEY ("sender_id") REFERENCES "users" ("user_id") ON DELETE CASCADE ON UPDATE NO ACTION,
  FOREIGN KEY ("receiver_id") REFERENCES "users" ("user_id") ON DELETE CASCADE ON UPDATE NO ACTION
);

PRAGMA foreign_keys = true;
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
-- ----------------------------
DROP TABLE IF EXISTS "blacklist";
CREATE TABLE "blacklist" (
  "block_id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "user_id" INTEGER NOT NULL,
  "admin_id" INTEGER NOT NULL,
  "reason" TEXT,
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY ("user_id") REFERENCES "users" ("user_id") ON DELETE NO ACTION ON UPDATE NO ACTION,
  FOREIGN KEY ("admin_id") REFERENCES "users" ("user_id") ON DELETE NO ACTION ON UPDATE NO ACTION
);

-- ----------------------------
-- Table structure for comments
-- ----------------------------
DROP TABLE IF EXISTS "comments";
CREATE TABLE "comments" (
  "comment_id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "user_id" INTEGER NOT NULL,
  "recipe_id" INTEGER NOT NULL,
  "content" TEXT NOT NULL,
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY ("user_id") REFERENCES "users" ("user_id") ON DELETE NO ACTION ON UPDATE NO ACTION,
  FOREIGN KEY ("recipe_id") REFERENCES "recipes" ("recipe_id") ON DELETE NO ACTION ON UPDATE NO ACTION
);

-- ----------------------------
-- Table structure for feedback
-- ----------------------------
DROP TABLE IF EXISTS "feedback";
CREATE TABLE "feedback" (
  "feedback_id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "user_id" INTEGER NOT NULL,
  "content" TEXT NOT NULL,
  "status" TEXT DEFAULT 'pending',
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY ("user_id") REFERENCES "users" ("user_id") ON DELETE NO ACTION ON UPDATE NO ACTION
);

-- ----------------------------
-- Table structure for recipes
-- ----------------------------
DROP TABLE IF EXISTS "recipes";
CREATE TABLE "recipes" (
  "recipe_id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "user_id" INTEGER NOT NULL,
  "title" TEXT NOT NULL,
  "content" TEXT NOT NULL,
  "category" TEXT,
  "type" TEXT,
  "video_path" TEXT,
  "likes" INTEGER DEFAULT 0,
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY ("user_id") REFERENCES "users" ("user_id") ON DELETE NO ACTION ON UPDATE NO ACTION
);

-- ----------------------------
-- Table structure for sqlite_sequence
-- ----------------------------
DROP TABLE IF EXISTS "sqlite_sequence";
CREATE TABLE "sqlite_sequence" (
  "name",
  "seq"
);

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS "users";
CREATE TABLE "users" (
  "user_id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "username" TEXT NOT NULL,
  "password" TEXT NOT NULL,
  "avatar_path" TEXT DEFAULT 'default_avatar.png',
  "signature" TEXT,
  "is_admin" BOOLEAN DEFAULT 0,
  "created_at" DATETIME DEFAULT CURRENT_TIMESTAMP,
  UNIQUE ("username" ASC)
);

PRAGMA foreign_keys = true;

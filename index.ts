import { createClient } from '@supabase/supabase-js';
import express from "express";

const supabase = createClient(
  "https://epbzyginbymtlaumsllx.supabase.co",
  "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImVwYnp5Z2luYnltdGxhdW1zbGx4Iiwicm9sZSI6ImFub24iLCJpYXQiOjE3NTc3NTgxNDQsImV4cCI6MjA3MzMzNDE0NH0.brg-BQq1UBQEtpT7nFVhUO4seYWDs76_NJnG2z_tt84"
);

const app = express();
app.use(express.json()); // <-- 解析 JSON

app.post("/saveapi", async (req, res) => {
  const json = req.body;

  const { data, error } = await supabase
    .from("json")
    .insert([{ data: json }]);

  if (error) {
    return res.status(500).send("儲存失敗QQ");
  } else {
    return res.status(200).send("儲存成功!");
  }
});

app.listen(3000, () => console.log("Server running on port 3000"));

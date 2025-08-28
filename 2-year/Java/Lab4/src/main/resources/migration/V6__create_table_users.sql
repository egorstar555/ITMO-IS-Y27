create table if not exists public.users (
   user_id bigserial primary key,
   name varchar (255) unique not null,
   password varchar(255) not null,
   role varchar (20) not null
);
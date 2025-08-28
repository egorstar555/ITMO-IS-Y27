package com.Starygin.data.repository;

import java.util.List;

public interface Repository<T> {
    public T save(T entity);

    public void deleteById(long id);

    public void deleteByEntity(T entity);

    public void deleteAll();

    public T update(T entity);

    public T getById(long id);

    public List<T> getAll();
}

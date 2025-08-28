package com.Starygin.data.repository;

import jakarta.persistence.EntityManager;

import java.util.List;

public class BaseRepository<T> implements Repository<T> {
    private final EntityManager entityManager;

    private final Class<T> entityClass;

    public BaseRepository(EntityManager entityManager, Class<T> entityClass) {
        this.entityManager = entityManager;
        this.entityClass = entityClass;
    }

    @Override
    public T save(T entity) {
        entityManager.persist(entity);

        return entity;
    }

    @Override
    public void deleteById(long id) {
        entityManager.remove(entityManager.find(entityClass, id));
        entityManager.flush();
    }

    @Override
    public void deleteByEntity(T entity) {
        entityManager.remove(entity);
        entityManager.flush();
    }

    @Override
    public void deleteAll() {
        var criteria = entityManager.getCriteriaBuilder().createCriteriaDelete(entityClass);
        criteria.from(entityClass);
        entityManager.createQuery(criteria).executeUpdate();
        entityManager.flush();
    }

    @Override
    public T update(T entity) {
        entityManager.merge(entity);
        return entity;
    }

    @Override
    public T getById(long id) {
        return entityManager.find(entityClass, id);
    }

    @Override
    public List<T> getAll() {
        var criteria = entityManager.getCriteriaBuilder().createQuery(entityClass);
        criteria.from(entityClass);

        return entityManager.createQuery(criteria).getResultList();
    }
}
